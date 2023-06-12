#pragma once

#include <QObject>
#include <QTimer>
#include <QThread>

#include "ConwayGame.h"
#include "ApplicationModel.h"

class ConwayWorker : public QObject
{
	Q_OBJECT

public:
	ConwayWorker(const ConwayGame& initialGame, ApplicationModel *appModel, QThread* thread);
private:
	void emitUpdatedState();

signals:
	void gameStateChanged(const ConwayGame::State& newState);
	void populationChanged(size_t population);
	void generationChanged(int generations);

public slots:
	void advanceOneGeneration();
	void setDelayMilliseconds(int delay);
	void setPaused(bool paused);
	void reset();
	void startTimer();
	void setGame(const ConwayGame& game, bool setNewInitialGame = true);
	void setAlive(long x, long y);


private:
	ConwayGame _game;
	ConwayGame _initialGame;
	QTimer* _timer;
	uint64_t _generations;
};
