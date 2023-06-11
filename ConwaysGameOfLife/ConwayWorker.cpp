#include "ConwayWorker.h"

ConwayWorker::ConwayWorker(const ConwayGame &game, 
	ApplicationModel *appModel, 
	QThread* thread): _game(game), _initialGame(game), _generations(0)
{
	this->moveToThread(thread);
	_timer = new QTimer;
	connect(_timer, &QTimer::timeout, this, &ConwayWorker::advanceOneGeneration);
	_timer->setInterval(appModel->getGenerationDurMs());
	_timer->moveToThread(thread);

	// Bind configuration signals to worker
	connect(appModel, &ApplicationModel::generationDurMsChanged, this, &ConwayWorker::setDelayMilliseconds);
	connect(appModel, &ApplicationModel::pausedChanged, this, &ConwayWorker::setPaused);
	connect(appModel, &ApplicationModel::advanceOneGeneration, this, &ConwayWorker::advanceOneGeneration);
	connect(appModel, &ApplicationModel::restart, this, &ConwayWorker::reset);

	// Bind output signals to model
	connect(this, &ConwayWorker::populationChanged, appModel, &ApplicationModel::setCurrentPopulation);
	connect(this, &ConwayWorker::generationChanged, appModel, &ApplicationModel::setGeneration);

	emitUpdatedState();
}

void ConwayWorker::setDelayMilliseconds(int delay)
{
	_timer->setInterval(delay);
}

void ConwayWorker::setPaused(bool paused)
{
	if (paused) _timer->stop();
	else _timer->start();
}

void ConwayWorker::reset()
{
	_game = _initialGame;
	emitUpdatedState();
}

void ConwayWorker::startTimer()
{
	_timer->start();
}

void ConwayWorker::setGame(const ConwayGame& game, bool setNewInitialGame)
{
	_game = game;
	if (setNewInitialGame) {
		_initialGame = game;
	}
	emitUpdatedState();
}

void ConwayWorker::advanceOneGeneration()
{
	_game.step();
	emitUpdatedState();
}

void ConwayWorker::emitUpdatedState()
{
	emit generationChanged(_game.getState().generations);
	emit populationChanged(_game.getState().board.size());
	emit gameStateChanged(_game.getState());
}