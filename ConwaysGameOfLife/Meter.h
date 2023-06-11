#pragma once

#include <QObject>
#include <QElapsedTimer>

class Meter : public QObject
{
	Q_OBJECT

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

