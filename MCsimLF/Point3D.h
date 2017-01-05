#pragma once

#include "Vector3D.h"

namespace MCsimLF {
	namespace MC {
		using namespace System;

		public value class Point3D
		{
			public:
				double x, y, z;

				Point3D(double _x, double _y, double _z);

				double getDistanceSquaredTo(Point3D point);
				double getDistanceTo(Point3D point);
				void add(Vector3D vector);
				void subtract(Vector3D vector);

				void applyPbc();

				virtual String^ ToString() override;
		};
	}
}