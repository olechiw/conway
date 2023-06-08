#pragma once

#include <QObject>
#include <QQmlEngine>

#include "Defaults.h"

class ApplicationModel : public QObject
{
	Q_OBJECT
	QML_SINGLETON
	QML_ELEMENT
	Q_PROPERTY(int simulationDelayMilliseconds MEMBER _simulationDelayMilliseconds NOTIFY simulationDelayChanged)
	Q_PROPERTY(bool paused MEMBER _paused WRITE setPaused NOTIFY pausedChanged)
	Q_PROPERTY(int currentPopulation MEMBER _currentPopulation WRITE setCurrentPopulation NOTIFY currentPopulationChanged)

public:
	ApplicationModel(QObject* parent = nullptr) : QObject(parent) {}
	~ApplicationModel() = default;

public slots:
	void setCurrentPopulation(int currentPopulation) {
		_currentPopulation = currentPopulation;
		emit currentPopulationChanged(_currentPopulation);
	}

	void setPaused(bool paused) {
		_paused = paused;
		emit pausedChanged(paused);
	}

signals:
	void simulationDelayChanged(const int &delay);
	void pausedChanged(const bool &paused);
	void currentPopulationChanged(const int& currentPopulation);

private:
	int _simulationDelayMilliseconds = DEFAULT_SIMULATION_DELAY_MILLISECONDS;
	bool _paused = DEFAULT_PAUSED_STATE;
	int _currentPopulation = DEFAULT_POPULATION;
};
