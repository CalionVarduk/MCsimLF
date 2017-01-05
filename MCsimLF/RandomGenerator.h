#pragma once

#include "Point3D.h"

namespace MCsimLF {
	namespace MC {
		using namespace System;

		public ref class RandomGenerator
		{
			public:
				RandomGenerator();
				RandomGenerator(UInt32 seed);

				double next();
				double next(double max);
				double next(double min, double max);
				Point3D nextPoint(Point3D min, Point3D max);

				UInt32 getSeed();

			private:
				static const UInt32 A = 16807;
				static const UInt32 B = 0;
				static const UInt32 C = 2147483647;

				UInt32 _seed;

				UInt32 _congruence();
		};
	}
}