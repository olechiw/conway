#pragma once

#include <QObject>
#include <QQmlEngine>

class ApplicationModel : public QObject
{
	Q_OBJECT
	QML_SINGLETON
	QML_ELEMENT
	Q_PROPERTY(int simulationDelay MEMBER _simulationDelay NOTIFY simulationDelayChanged)
	Q_PROPERTY(bool paused MEMBER _paused NOTIFY pausedChanged)

public:
	ApplicationModel(QObject *parent = nullptr);
	~ApplicationModel();

signals:
	void simulationDelayChanged(const int &delay);
	void pausedChanged(const bool &paused);

	// starting configuration property
private:
	int _simulationDelay = 0;
	bool _paused = false;
};
