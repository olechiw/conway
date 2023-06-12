#include "Meter.h"

void Meter::increment() {
    incrementBy(1);
}

void Meter::incrementBy(uint64_t value) {
    _totalDuration += _timer.elapsed();
    _count += value;
    if (_totalDuration > UPDATE_TIME_MS) {
        emit onMeterUpdated((1000.0 * _count) / _totalDuration);
        _totalDuration = 0;
        _count = 0;
    }
    _timer.restart();
}

Meter::Meter()
{
    _timer.start();
}
