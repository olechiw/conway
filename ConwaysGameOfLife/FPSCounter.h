#pragma once

#include <QObject>
#include <QQuickItem>
#include <QElapsedTimer>

class FPSCounter : public QQuickItem
{
	Q_OBJECT

public:
	explicit FPSCounter(QQuickItem* parent = nullptr);

public slots:
	void logFrame();

signals:
	void fpsUpdated(double newFPS);

private:
	QElapsedTimer _timer;
	int _frameCount = 0;
	int _totalDuration = 0;
};

