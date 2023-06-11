#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QBindable>

#include "Defaults.h"

#define Q_SIMPLE_BOUND_PROPERTY(className, type, camelCaseName, pascalCaseName, defaultValue) \
	private: \
		Q_PROPERTY(type camelCaseName READ get##pascalCaseName WRITE set##pascalCaseName BINDABLE b##pascalCaseName) \
	public: \
		Q_SIGNAL void camelCaseName##Changed(type camelCaseName); \
	public slots: \
		void set##pascalCaseName##(type camelCaseName) { _b##pascalCaseName = camelCaseName; } \
		type get##pascalCaseName##() { return _b##pascalCaseName; } \
	public: \
		QBindable<##type##> b##pascalCaseName() { return &_b##pascalCaseName; } \
		Q_OBJECT_BINDABLE_PROPERTY_WITH_ARGS(className, type, _b##pascalCaseName##, defaultValue, &##className##::##camelCaseName##Changed)

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
