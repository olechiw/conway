#pragma once

#include <QObject>
#include <QTimer>
#include <QThread>

#include "ConwayGame.h"
#include "Defaults.h"

class ConwayWorker : public QObject
{
	Q_OBJECT

public:
	ConwayWorker(ConwayGame* game, QThread* thread);

signals:
	void gameUpdated();
	void renderFrame(const ConwayGame::State& newState);

public slots:
	void step();
	void setDelayMilliseconds(int delay);
	void setPaused(bool paused);
	void reset();
	void startTimer();

private:
	ConwayGame* _game;
	QTimer* _timer;
	bool _paused;
};
