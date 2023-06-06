#pragma once

#include <QThread>

#include "ConwayGame.h"

class ConwayWorker  : public QThread
{
	Q_OBJECT

public:
	ConwayWorker(ConwayGame* game);

signals:
	void gameUpdated(const ConwayGame::State& newState);

private:
	void run() override;

	ConwayGame* _game;
};
