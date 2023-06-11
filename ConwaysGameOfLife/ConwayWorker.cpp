#include "ConwayWorker.h"

ConwayWorker::ConwayWorker(ConwayGame* game, QThread* thread): _game(game), _generations(0)
{
	// TODO: default configurations
	this->moveToThread(thread);
	_timer = new QTimer;
	connect(_timer, SIGNAL(timeout()), this, SLOT(advanceOneGeneration()));
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
	emit setGenerations(++_generations);
	emit setPopulation(_game->getState().board.size());
	emit setGameState(_game->getState());
}