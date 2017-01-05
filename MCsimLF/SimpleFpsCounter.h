#pragma once

namespace MCsimLF {
	namespace Helpers {
		using namespace System;
		using namespace System::Diagnostics;

		public delegate void FpsCounterUpdateHandler();

		public ref class SimpleFpsCounter
		{
			public:
				FpsCounterUpdateHandler^ FrameCountUpdate;

				property UInt32 UpdateInterval {
					public: UInt32 get() { return (UInt32)_updateInterval; }
					public: void set(UInt32 value) { _updateInterval = (Int32)value; }
				}

				SimpleFpsCounter();

				void countFrame();
				float getFpsCount();

			private:
				Stopwatch^ _stopwatch;
				Int64 _prevUpdate;
				Int32 _ticksPerUpdate;
				Int32 _updateInterval;
				float _framesPerSecond;
		};
	}
}