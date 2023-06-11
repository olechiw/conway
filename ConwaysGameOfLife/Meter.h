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

signals:
	void meterUpdated(double newFPS);

private:
	QElapsedTimer _timer;
	int _frameCount = 0;
	int _totalDuration = 0;
};

