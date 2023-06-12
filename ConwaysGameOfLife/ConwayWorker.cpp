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

	// Worker is designed to run at extremely high frequency and so it shouldn't be doing any signal emission
	// connect(this, &ConwayWorker::populationChanged, appModel, &ApplicationModel::setCurrentPopulation);
	// connect(this, &ConwayWorker::generationChanged, appModel, &ApplicationModel::setGeneration);

	setState();
}

ConwayWorker::~ConwayWorker()
{
	_timer->stop();
	delete _timer;
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
	setState();
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
	setState();
}

void ConwayWorker::setAlive(int64_t x, int64_t y)
{
	_game.setAlive(x, y);
	setState();
}

void ConwayWorker::advanceOneGeneration()
{
	_game.step();
	setState();
}

void ConwayWorker::setState()
{
	QMutexLocker locker(&_stateMutex);
	_latestState = _game.getState();
}

ConwayGame::State ConwayWorker::getState()
{
	QMutexLocker locker(&_stateMutex);
	return _latestState;
}