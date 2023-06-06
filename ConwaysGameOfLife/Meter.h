#pragma once

#include <QObject>
#include <QQuickItem>
#include <QElapsedTimer>

class Meter : public QQuickItem
{
	Q_OBJECT

public:
	explicit Meter(QQuickItem* parent = nullptr);

public slots:
	void increment();

signals:
	void meterUpdated(double newFPS);

private:
	QElapsedTimer _timer;
	int _frameCount = 0;
	int _totalDuration = 0;
};

