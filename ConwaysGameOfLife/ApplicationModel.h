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
	Q_PROPERTY(bool paused MEMBER _paused NOTIFY pausedChanged)

public:
	ApplicationModel(QObject* parent = nullptr) : QObject(parent) {}
	~ApplicationModel() = default;

signals:
	void simulationDelayChanged(const int &delay);
	void pausedChanged(const bool &paused);

private:
	int _simulationDelayMilliseconds = DEFAULT_SIMULATION_DELAY_MILLISECONDS;
	bool _paused = DEFAULT_PAUSED_STATE;
};
