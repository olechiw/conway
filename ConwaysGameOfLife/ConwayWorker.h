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
	void setGameState(const ConwayGame::State& newState);
	void setPopulation(size_t population);
	void setGenerations(int generations);

public slots:
	void advanceOneGeneration();
	void setDelayMilliseconds(int delay);
	void setPaused(bool paused);
	void reset();
	void startTimer();


private:
	ConwayGame* _game;
	QTimer* _timer;
	uint64_t _generations;
};
