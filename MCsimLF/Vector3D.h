#pragma once

#include "SimSettings.h"

namespace MCsimLF {
	namespace MC {
		using namespace System;

		public value class Vector3D
		{
			public:
				double x, y, z;

				Vector3D(double _x, double _y, double _z);

				double getLengthSquared();
				double getLength();

				Vector3D getNormalised();
				Vector3D getNegated();

				void applyPbc();

				virtual String^ ToString() override;
		};
	}
}