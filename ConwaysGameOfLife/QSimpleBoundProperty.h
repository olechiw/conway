#pragma once

#include <QObject>
#include <QBindable>

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