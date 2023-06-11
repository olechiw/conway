#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QBindable>

#include "Defaults.h"
#include "QSimpleBoundProperty.h"


class ApplicationModel : public QObject
{
	Q_OBJECT
	QML_SINGLETON
	QML_ELEMENT

	Q_SIMPLE_BOUND_PROPERTY(ApplicationModel, int, generationDurMs, GenerationDurMs, DEFAULT_GENERATION_DURATION_MILLISECONDS)
	Q_SIMPLE_BOUND_PROPERTY(ApplicationModel, int, paused, Paused, DEFAULT_PAUSED_STATE)

	Q_SIMPLE_BOUND_PROPERTY(ApplicationModel, int, currentPopulation, CurrentPopulation, 0)
	Q_SIMPLE_BOUND_PROPERTY(ApplicationModel, double, currentFps, CurrentFps, 0.0)
	Q_SIMPLE_BOUND_PROPERTY(ApplicationModel, double, generationsPerSecond, GenerationsPerSecond, 0.0)
	Q_SIMPLE_BOUND_PROPERTY(ApplicationModel, uint64_t, generations, Generations, 0)

	Q_SIGNAL void advanceOneGeneration();
};
