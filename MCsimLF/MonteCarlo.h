#pragma once

#include "RandomGenerator.h"
#include "SimFileManager.h"

namespace MCsimLF {
	namespace MC {
		using namespace System;
		using namespace System::Collections::Generic;

		using namespace MCsimLF::FileManip;

		public ref class MonteCarlo
		{
			public:
				static const double sigma = 3.4;
				static const double epsilon = 1.6;
				static const double constBoltzmann = 0.0138;

				static UInt32 getFccCount(array<Int32>^ fccParticleCount);

				MonteCarlo();

				void initializeNormal(UInt32 atomCount, Vector3D boxSize, double temperature, Point3D origin);
				void initializeFcc(array<Int32>^ fccAtomCount, Vector3D boxSize, double temperature, Point3D origin);

				double calculatePotential(UInt32 iAtom1, UInt32 iAtom2);
				double calculateAtomEnergy(UInt32 iAtom);
				double calculateEnergy();

				void metropolisStep();

				Vector3D getInitialBoxSize();
				double getInitialBoxVolume();
				double getTemperature();
				UInt32 getAtomCount();
				Point3D getAtom(UInt32 iAtom);
				Point3D getOrigin();

				UInt32 getStep();
				double getEnergy();

				double getEnergyValueAt(UInt32 iStep);

				UInt32 getEnergySyncInterval();
				void setEnergySyncInterval(UInt32 interval);

				bool saveToFile(String^ path);
				bool loadFromFile(String^ path);

			private:
				array<Point3D>^ _atoms;
				List<double>^ _energyArchive;
				RandomGenerator^ _generator;
				Vector3D _boxSize;
				double _temperature, _thermalBalance;
				double _energy;
				UInt32 _step;
				UInt32 _energySyncStep, _energySyncInterval;
				Point3D _origin;

				void _initializeBoxProperties(Vector3D boxSize, Point3D origin);
				void _initializeTemperature(double temperature);
				void _initializeMetropolisVariables(UInt32 step, double energy, UInt32 energySyncInterval);

				double _uncheckedCalculatePotential(UInt32 iAtom1, UInt32 iAtom2);
				void _updateEnergy(double energyDelta);

				array<Byte>^ _toByteArray();
				bool _fromByteArray(array<Byte>^ mcArray);

				static void __uintToByteArray(UInt32 number, array<Byte>^ mcArray, UInt32% iByte);
				static void __doubleToByteArray(double number, array<Byte>^ mcArray, UInt32% iByte);
				static UInt32 __byteArrayToUint(array<Byte>^ mcArray, UInt32% iByte);
				static double __byteArrayToDouble(array<Byte>^ mcArray, UInt32% iByte);
		};
	}
}