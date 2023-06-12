#pragma once

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QMutex>

#include "ConwayGame.h"
#include "ApplicationModel.h"

class ConwayWorker : public QObject
{
	Q_OBJECT

public:
	ConwayWorker(const ConwayGame& initialGame, ApplicationModel *appModel, QThread* thread);
	~ConwayWorker();
private:
	void setState();

public slots:
	void advanceOneGeneration();
	void setDelayMilliseconds(int delay);
	void setPaused(bool paused);
	void reset();
	void startTimer();
	void setGame(const ConwayGame& game, bool setNewInitialGame = true);
	void setAlive(int64_t x, int64_t y);

	// Thread safe for direct connections
	ConwayGame::State getState();


private:
	ConwayGame _game;
	ConwayGame _initialGame;
	QTimer* _timer;
	uint64_t _generations;

	QMutex _stateMutex;
	ConwayGame::State _latestState;
};