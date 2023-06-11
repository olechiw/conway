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
	connect(this, &ConwayWorker::setPopulation, appModel, &ApplicationModel::setCurrentPopulation);
	connect(this, &ConwayWorker::setGenerations, appModel, &ApplicationModel::setGenerations);

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

void ConwayWorker::advanceOneGeneration()
{
	_game.step();
	emitUpdatedState();
}

void ConwayWorker::emitUpdatedState()
{
	emit setGenerations(_game.getState().generations);
	emit setPopulation(_game.getState().board.size());
	emit setGameState(_game.getState());
}