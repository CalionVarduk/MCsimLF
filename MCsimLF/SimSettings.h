#pragma once

namespace MCsimLF
{
	public ref class SimSettings abstract
	{
		public:
			static bool fccOn = false;
			static bool pbcOn = false;

			static double boxOriginX = 0;
			static double boxOriginY = 0;
			static double boxOriginZ = 0;

			static double boxLengthX = 10;
			static double boxLengthY = 10;
			static double boxLengthZ = 10;
			static double boxLengthHalfX = boxLengthX * 0.5;
			static double boxLengthHalfY = boxLengthY * 0.5;
			static double boxLengthHalfZ = boxLengthZ * 0.5;
	};
}