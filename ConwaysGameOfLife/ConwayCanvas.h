#pragma once
#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QSGSimpleRectNode>

#include "ConwayGame.h"

class ConwayCanvas :
    public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    ConwayCanvas(QQuickItem* parent = nullptr);

    QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData*) override;

public slots:
    void nextGameState(const ConwayGame::State& state);

private:
    ConwayGame::State _currentState;
};

