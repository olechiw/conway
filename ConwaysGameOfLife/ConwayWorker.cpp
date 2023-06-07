#include "ConwayWorker.h"

ConwayWorker::ConwayWorker(ConwayGame* game, QThread* thread): _game(game), _paused(false)
{
	// TODO: default configurations
	this->moveToThread(thread);
	_timer = new QTimer;
	connect(_timer, SIGNAL(timeout()), this, SLOT(step()));
	connect(thread, SIGNAL(started()), this, SLOT(startTimer()));
	_timer->setInterval(0);
	_timer->moveToThread(thread);
}

void ConwayWorker::setDelayMilliseconds(int delay)
{
	_timer->setInterval(delay);
}

void ConwayWorker::setPaused(bool paused)
{
	_paused = paused;
}

void ConwayWorker::reset()
{
	// TODO: default configurations
}

void ConwayWorker::startTimer()
{
	_timer->start();
}

void ConwayWorker::step()
{
	if (_paused) return;
	_game->step();
	emit gameUpdated(_game->getState());
}
