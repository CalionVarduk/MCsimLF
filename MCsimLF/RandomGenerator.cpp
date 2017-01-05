#include "RandomGenerator.h"

using namespace MCsimLF::MC;

RandomGenerator::RandomGenerator()
{
	DateTime time = DateTime::Now;

	_seed = (time.Millisecond + (UInt32)(time.Second + time.Minute * 60 +
			time.Hour * 3600 + time.DayOfYear * 86400) * 1000U) % C;
}

RandomGenerator::RandomGenerator(UInt32 seed)
{
	_seed = (seed > 0) ? seed % C : 0;
}

double RandomGenerator::next()
{
	static const double invC = 1.0 / C;
	return _congruence() * invC;
}

double RandomGenerator::next(double max)
{
	return next() * max;
}

double RandomGenerator::next(double min, double max)
{
	double delta = max - min;
	return (delta > 0) ? next(delta) + min : 0;
}

Point3D RandomGenerator::nextPoint(Point3D min, Point3D max)
{
	return Point3D(next(min.x, max.x), next(min.y, max.y), next(min.z, max.z));
}

UInt32 RandomGenerator::getSeed()
{
	return _seed;
}

UInt32 RandomGenerator::_congruence()
{
	return (_seed = (A * _seed + B) % C);
}