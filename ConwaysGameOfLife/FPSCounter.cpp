#include "FPSCounter.h"

void FPSCounter::logFrame() {
    _totalDuration += _timer.elapsed();
    ++_frameCount;
    if (_totalDuration > 250) {
        emit fpsUpdated((1000.0 * _frameCount) / _totalDuration);
        _totalDuration = 0;
        _frameCount = 0;
    }
    _timer.restart();
}

FPSCounter::FPSCounter(QQuickItem* parent)
{
    _timer.start();
}
