#include "SimpleFpsCounter.h"

using namespace MCsimLF::Helpers;

SimpleFpsCounter::SimpleFpsCounter()
{
	FrameCountUpdate = nullptr;
	_updateInterval = 1000;
	_prevUpdate = 0;
	_ticksPerUpdate = 0;
	_framesPerSecond = 0;

	_stopwatch = gcnew Stopwatch();
	_stopwatch->Start();
}

void SimpleFpsCounter::countFrame()
{
	++_ticksPerUpdate;
	Int64 currentUpdate = _stopwatch->ElapsedMilliseconds;
	Int32 timeDifference = (Int32)(currentUpdate - _prevUpdate);

	if(timeDifference >= _updateInterval) {
		_framesPerSecond = (_ticksPerUpdate * 1000) / (float)timeDifference;
		_ticksPerUpdate = 0;
		_prevUpdate = currentUpdate;

		if(FrameCountUpdate != nullptr)
			FrameCountUpdate->Invoke();
	}
}

float SimpleFpsCounter::getFpsCount()
{
	return _framesPerSecond;
}