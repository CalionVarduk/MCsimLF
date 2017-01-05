#include "MonteCarlo.h"

using namespace MCsimLF::MC;

UInt32 MonteCarlo::getFccCount(array<Int32>^ fccParticleCount)
{
	Byte EvenCounter = 0;
	for(Int32 i = 0; i < fccParticleCount->Length; ++i)
		if((fccParticleCount[i] & 1) == 0)
			++EvenCounter;

	Int32 posX = (fccParticleCount[0] << 1) + 1;
	Int32 posY = (fccParticleCount[1] << 1) + 1;
	Int32 posZ = (fccParticleCount[2] << 1) + 1;

	return ((posX * posY * posZ) >> 1) + ((EvenCounter & 1) ? 1 : 0);
}

MonteCarlo::MonteCarlo()
{
	_energyArchive = gcnew List<Double>(0);
	_generator = gcnew RandomGenerator();
	_atoms = gcnew array<Point3D>(0);

	_initializeBoxProperties(Vector3D(), Point3D());
	_initializeTemperature(0);
	_initializeMetropolisVariables(0, 0, 100);
}

void MonteCarlo::initializeNormal(UInt32 atomCount, Vector3D boxSize, double temperature, Point3D origin)
{
	_energyArchive->Clear();
	_energyArchive = gcnew List<Double>(1000000);
	_generator = gcnew RandomGenerator();

	_initializeBoxProperties(boxSize, origin);
	_initializeTemperature(temperature);
	_initializeMetropolisVariables(0, 0, 100);

	Point3D boundMin = Point3D(_origin.x - (_boxSize.x * 0.5), _origin.y - (_boxSize.y * 0.5), _origin.z - (_boxSize.z * 0.5));
	Point3D boundMax = Point3D(_origin.x + (_boxSize.x * 0.5), _origin.y + (_boxSize.y * 0.5), _origin.z + (_boxSize.z * 0.5));

	_atoms = gcnew array<Point3D>(atomCount);
	for(Int32 i = 0; i < _atoms->Length; ++i)
		_atoms[i] = _generator->nextPoint(boundMin, boundMax);

	_energy = calculateEnergy();
	_energyArchive->Add(_energy);
}

void MonteCarlo::initializeFcc(array<Int32>^ fccAtomCount, Vector3D boxSize, double temperature, Point3D origin)
{
	if(fccAtomCount->Length != 3)
		return;

	_energyArchive->Clear();
	_energyArchive = gcnew List<Double>(1000000);
	_generator = gcnew RandomGenerator();

	_initializeBoxProperties(boxSize, origin);
	_initializeTemperature(temperature);
	_initializeMetropolisVariables(0, 0, 100);

	Vector3D distance = Vector3D(boxSize.x / ((fccAtomCount[0] << 1) + 1),
								boxSize.y / ((fccAtomCount[1] << 1) + 1),
								boxSize.z / ((fccAtomCount[2] << 1) + 1));

	UInt32 i = 0;
	_atoms = gcnew array<Point3D>(getFccCount(fccAtomCount));

	for(Int32 x = -fccAtomCount[0]; x <= fccAtomCount[0]; ++x)
		for(Int32 y = -fccAtomCount[1]; y <= fccAtomCount[1]; ++y)
			for(Int32 z = -fccAtomCount[2]; z <= fccAtomCount[2]; ++z)
				if(((x + y + z) & 1) == 0)
					_atoms[i++] = Point3D(_origin.x + (distance.x * x), _origin.y + (distance.y * y), _origin.z + (distance.z * z));

	_energy = calculateEnergy();
	_energyArchive->Add(_energy);
}

double MonteCarlo::calculatePotential(UInt32 iAtom1, UInt32 iAtom2)
{
	return ((iAtom1 < (UInt32)_atoms->Length) && (iAtom2 < (UInt32)_atoms->Length)) ?
			_uncheckedCalculatePotential(iAtom1, iAtom2) :
			0;
}

double MonteCarlo::calculateAtomEnergy(UInt32 iAtom)
{
	if(iAtom < (UInt32)_atoms->Length) {
		double energy = 0.0;

		for(UInt32 j = 0; j < iAtom; ++j)
			energy += _uncheckedCalculatePotential(iAtom, j);

		for(Int32 j = iAtom + 1; j < _atoms->Length; ++j)
			energy += _uncheckedCalculatePotential(iAtom, j);

		return energy;
	}
	return 0;
}

double MonteCarlo::calculateEnergy()
{
	double energy = 0.0;
	Int32 particlesLengthM1 = _atoms->Length - 1;

	for(Int32 i = 0; i < particlesLengthM1; ++i)
		for(Int32 j = i + 1; j < _atoms->Length; ++j)
				energy += _uncheckedCalculatePotential(i, j);

	return energy * 2;
}

void MonteCarlo::metropolisStep()
{
	static const double multiplier = 0.2;

	++_step;
	UInt32 iAtom = _generator->getSeed() % _atoms->Length;
	Vector3D move(_generator->next(-multiplier, multiplier),
				_generator->next(-multiplier, multiplier),
				_generator->next(-multiplier, multiplier));

	double prevE = calculateAtomEnergy(iAtom);
	_atoms[iAtom].add(move);
	double deltaE = calculateAtomEnergy(iAtom) - prevE;

	if(deltaE > 0) {
		double factorBoltzmann = Math::Exp(-_thermalBalance * deltaE);
		double dzeta = _generator->next();

		if(dzeta > factorBoltzmann) {
			_atoms[iAtom].subtract(move);
			_energyArchive->Add(_energy);
			return;
		}
	}

	_updateEnergy(deltaE);
}

inline Vector3D MonteCarlo::getInitialBoxSize()
{ return _boxSize; }

inline double MonteCarlo::getInitialBoxVolume()
{ return _boxSize.x * _boxSize.y * _boxSize.z; }

inline double MonteCarlo::getTemperature()
{ return _temperature; }

inline UInt32 MonteCarlo::getAtomCount()
{ return _atoms->Length; }

inline Point3D MonteCarlo::getAtom(UInt32 iAtom)
{
	return (iAtom < (UInt32)_atoms->Length) ?
			_atoms[iAtom] :
			Point3D();
}

inline Point3D MonteCarlo::getOrigin()
{ return _origin; }

inline UInt32 MonteCarlo::getStep()
{ return _step; }

inline double MonteCarlo::getEnergy()
{ return _energy; }

inline double MonteCarlo::getEnergyValueAt(UInt32 iStep)
{ return (iStep <= _step) ? _energyArchive[iStep] : 0; }

inline UInt32 MonteCarlo::getEnergySyncInterval()
{ return _energySyncInterval; }

inline void MonteCarlo::setEnergySyncInterval(UInt32 interval)
{ _energySyncInterval = (interval > 0) ? interval : 1; }

bool MonteCarlo::saveToFile(String^ path)
{
	SimFileManager^ fileMgr = gcnew SimFileManager();
	return fileMgr->save(path, _toByteArray());
}

bool MonteCarlo::loadFromFile(String^ path)
{
	SimFileManager^ fileMgr = gcnew SimFileManager();
	return _fromByteArray(fileMgr->load(path));
}

inline void MonteCarlo::_initializeBoxProperties(Vector3D boxSize, Point3D origin)
{
	_boxSize = Vector3D((boxSize.x > 0.01) ? boxSize.x : 0.01, (boxSize.y > 0.01) ? boxSize.y : 0.01, (boxSize.z > 0.01) ? boxSize.z : 0.01);
	_origin = origin;
}

inline void MonteCarlo::_initializeTemperature(double temperature)
{
	_temperature = (temperature > 0) ? temperature : 0;
	_thermalBalance = 1 / (constBoltzmann * _temperature);
}

inline void MonteCarlo::_initializeMetropolisVariables(UInt32 step, double energy, UInt32 energySyncInterval)
{
	_step = step;
	_energy = energy;
	_energySyncStep = 0;
	_energySyncInterval = energySyncInterval;
}

inline double MonteCarlo::_uncheckedCalculatePotential(UInt32 iAtom1, UInt32 iAtom2)
{
	static const double epsilon4 = 4 * epsilon;
	static const double sigma2 = sigma * sigma;

	double s2d2 = sigma2 / _atoms[iAtom1].getDistanceSquaredTo(_atoms[iAtom2]);
	s2d2 *= s2d2 * s2d2;
	return epsilon4 * (s2d2 * s2d2 - s2d2);
}

inline void MonteCarlo::_updateEnergy(double energyDelta)
{
	if(++_energySyncStep < _energySyncInterval)
		_energy += energyDelta * 2;
	else {
		_energy = calculateEnergy();
		_energySyncStep = 0;
	}
	_energyArchive->Add(_energy);
}

array<Byte>^ MonteCarlo::_toByteArray()
{
	UInt32 size = 4 +									// atom count (int32)
					1 +									// pbc flag (bool)
					8 +									// temperature (double)
					24 +								// box size (3x double)
					24 +								// origin (3x double)
					4 +									// steps elapsed (int32)
					(8 * _energyArchive->Count) +		// energy for each elapsed step (doubles)
					(24 * _atoms->Length);				// atom positions (doubles)

	UInt32 i = 0;
	array<Byte>^ mcArray = gcnew array<Byte>(size);

	__uintToByteArray(_atoms->Length, mcArray, i);
	mcArray[i++] = (SimSettings::pbcOn) ? 1 : 0;
	__doubleToByteArray(_temperature, mcArray, i);
	__doubleToByteArray(_boxSize.x, mcArray, i);
	__doubleToByteArray(_boxSize.y, mcArray, i);
	__doubleToByteArray(_boxSize.z, mcArray, i);
	__doubleToByteArray(_origin.x, mcArray, i);
	__doubleToByteArray(_origin.y, mcArray, i);
	__doubleToByteArray(_origin.z, mcArray, i);
	__uintToByteArray(_step, mcArray, i);

	for(Int32 j = 0; j < _energyArchive->Count; ++j)
		__doubleToByteArray(_energyArchive[j], mcArray, i);

	for(Int32 j = 0; j < _atoms->Length; ++j) {
		__doubleToByteArray(_atoms[j].x, mcArray, i);
		__doubleToByteArray(_atoms[j].y, mcArray, i);
		__doubleToByteArray(_atoms[j].z, mcArray, i);
	}

	return mcArray;
}

bool MonteCarlo::_fromByteArray(array<Byte>^ mcArray)
{
	if(mcArray->Length < 65)
		return false;

	UInt32 i = 0;
	UInt32 atomCount = __byteArrayToUint(mcArray, i);
	bool pbcOn = (mcArray[i++] == 1);
	double temperature = __byteArrayToDouble(mcArray, i);
	double sX = __byteArrayToDouble(mcArray, i);
	double sY = __byteArrayToDouble(mcArray, i);
	double sZ = __byteArrayToDouble(mcArray, i);
	double pX = __byteArrayToDouble(mcArray, i);
	double pY = __byteArrayToDouble(mcArray, i);
	double pZ = __byteArrayToDouble(mcArray, i);
	Vector3D boxSize = Vector3D(sX, sY, sZ);
	Point3D origin = Point3D(pX, pY, pZ);
	UInt32 step = __byteArrayToUint(mcArray, i);

	if(mcArray->Length != (65 + (8 * (step + 1)) + (24 * atomCount)))
		return false;

	_energyArchive = gcnew List<double>(1000000);
	for(UInt32 j = 0; j <= step; ++j)
		_energyArchive->Add(__byteArrayToDouble(mcArray, i));

	_atoms = gcnew array<Point3D>(atomCount);
	for(UInt32 j = 0; j < atomCount; ++j) {
		pX = __byteArrayToDouble(mcArray, i);
		pY = __byteArrayToDouble(mcArray, i);
		pZ = __byteArrayToDouble(mcArray, i);
		_atoms[j] = Point3D(pX, pY, pZ);
	}

	_initializeBoxProperties(boxSize, origin);
	_initializeTemperature(temperature);
	_initializeMetropolisVariables(step, _energyArchive[step], 100);
	SimSettings::pbcOn = pbcOn;

	return true;
}

inline void MonteCarlo::__uintToByteArray(UInt32 number, array<Byte>^ mcArray, UInt32% iByte)
{
	mcArray[iByte++] = (Byte)(number & 255);
	mcArray[iByte++] = (Byte)((number >> 8) & 255);
	mcArray[iByte++] = (Byte)((number >> 16) & 255);
	mcArray[iByte++] = (Byte)(number >> 24);
}

inline void MonteCarlo::__doubleToByteArray(double number, array<Byte>^ mcArray, UInt32% iByte)
{
	array<Byte>^ doubleBytes = BitConverter::GetBytes(number);
	for(UInt32 j = 0; j < 8; ++j)
		mcArray[iByte++] = doubleBytes[j];
}

inline UInt32 MonteCarlo::__byteArrayToUint(array<Byte>^ mcArray, UInt32% iByte)
{
	UInt32 value = mcArray[iByte++];
	value |= (UInt32)mcArray[iByte++] << 8;
	value |= (UInt32)mcArray[iByte++] << 16;
	value |= (UInt32)mcArray[iByte++] << 24;
	return value;
}

inline double MonteCarlo::__byteArrayToDouble(array<Byte>^ mcArray, UInt32% iByte)
{
	double value = BitConverter::ToDouble(mcArray, iByte);
	iByte += 8;
	return value;
}