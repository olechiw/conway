#pragma once
#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QSGSimpleRectNode>

#include <memory>
#include <memory_resource>

#include "ConwayGame.h"
#include "QSimpleBoundProperty.h"

class ConwayCanvas :
    public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    ConwayCanvas(QQuickItem* parent = nullptr);

    QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData*) override;

public slots:
    void setGameState(const ConwayGame::State& state);

private:
    ConwayGame::State _currentState;
    Q_SIMPLE_BOUND_PROPERTY(ConwayCanvas, bool, drawGridLines, DrawGridLines, false);
};