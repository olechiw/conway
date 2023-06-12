#pragma once

#include <QObject>
#include <QElapsedTimer>

class Meter : public QObject
{
	Q_OBJECT

	static constexpr int UPDATE_TIME_MS = 100;
public:
	Meter();

public slots:
	void increment();
	void incrementBy(uint64_t value);

signals:
	void onMeterUpdated(double value);

private:
	QElapsedTimer _timer;
	uint64_t _count = 0;
	uint64_t _totalDuration = 0;
};

