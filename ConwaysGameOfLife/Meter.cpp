#include "Meter.h"

void Meter::increment() {
    _totalDuration += _timer.elapsed();
    ++_frameCount;
    if (_totalDuration > UPDATE_TIME_MS) {
        emit meterUpdated((1000.0 * _frameCount) / _totalDuration);
        _totalDuration = 0;
        _frameCount = 0;
    }
    _timer.restart();
}

Meter::Meter()
{
    _timer.start();
}
