#include "GameStatsReporter.h"

GameStatsReporter::GameStatsReporter(ApplicationModel* appModel)
{
	connect(this, &GameStatsReporter::updateGeneration, appModel, &ApplicationModel::setGeneration);
	connect(&_generationsPerSecondMeter, &Meter::onMeterUpdated, appModel, &ApplicationModel::setGenerationsPerSecond);
	connect(this, &GameStatsReporter::updatePopulation, appModel, &ApplicationModel::setCurrentPopulation);

	connect(&_timer, &QTimer::timeout, this, &GameStatsReporter::report);
	_timer.setInterval(GAME_STATS_TIME_MS);
	_timer.start();
}

void GameStatsReporter::report() 
{
	ConwayGame::State state = emit requestState();
	emit updateGeneration(state.generations);
	emit updatePopulation(state.board.size());
	if (state.generations > _lastState.generations)
		_generationsPerSecondMeter.incrementBy(state.generations - _lastState.generations);
	_lastState = state;
}
