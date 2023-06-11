#include "ConwayWorker.h"

ConwayWorker::ConwayWorker(ConwayGame* game, QThread* thread): _game(game), _generations(0)
{
	// TODO: default configurations
	this->moveToThread(thread);
	_timer = new QTimer;
	connect(_timer, SIGNAL(timeout()), this, SLOT(advanceOneGeneration()));
	// connect(thread, SIGNAL(started()), this, SLOT(startTimer()));
	_timer->setInterval(DEFAULT_GENERATION_DURATION_MILLISECONDS);
	_timer->moveToThread(thread);
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
	emit setGenerations(0);
}

void ConwayWorker::startTimer()
{
	_timer->start();
}

void ConwayWorker::advanceOneGeneration()
{
	_game->step();
	emit gameWasUpdated();
	emit setGenerations(++_generations);
	emit currentPopulation(_game->getState().board.size());
	emit nextGameState(_game->getState());
}