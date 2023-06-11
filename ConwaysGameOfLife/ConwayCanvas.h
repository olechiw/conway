#pragma once
#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QSGSimpleRectNode>
#include <QTimer>

#include "ConwayGame.h"
#include "QSimpleBoundProperty.h"

class ConwayCanvas :
    public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT

    static constexpr int FRAME_TIME_MS = 4;
public:
    ConwayCanvas(QQuickItem* parent = nullptr);

    QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData*) override;

public slots:
    void gameStateChanged(const ConwayGame::State& state);

private:
    ConwayGame::State _currentState;
    QTimer _renderTimer;
    Q_SIMPLE_BOUND_PROPERTY(ConwayCanvas, bool, drawGridLines, DrawGridLines, false);
};