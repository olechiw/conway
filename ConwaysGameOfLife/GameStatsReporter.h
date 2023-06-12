#pragma once

#include <QObject>
#include <QTimer>

#include "ConwayGame.h"
#include "ApplicationModel.h"
#include "Meter.h"

class GameStatsReporter : public QObject
{
	Q_OBJECT
	static constexpr int GAME_STATS_TIME_MS = 100;
public:
	explicit GameStatsReporter(ApplicationModel* appModel);

private slots:
	void report();

signals:
	ConwayGame::State requestLatestState();
	void updateGenerationsPerSecond(double generationsPerSecond);
	void updateGeneration(uint64_t generation);
	void updatePopulation(size_t population);

private:
	QTimer _timer{};
	Meter _generationsPerSecondMeter{};
	ConwayGame::State _lastState{};
};

