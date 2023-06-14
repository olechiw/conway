#pragma once
#include <QColor>
#include <QSGSimpleRectNode>
#include <QTimer>
#include <QQuickItem>

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
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void onClicked(int64_t x, int64_t y);
    ConwayGame::State requestLatestState();

private:
    Q_SIMPLE_BOUND_PROPERTY(ConwayCanvas, bool, drawGridLines, DrawGridLines, false);
    Q_SIMPLE_BOUND_PROPERTY(ConwayCanvas, uint64_t, userViewMinimumWidth, UserViewMinimumWidth, 5);
    Q_SIMPLE_BOUND_PROPERTY(ConwayCanvas, uint64_t, userViewMinimumHeight, UserViewMinimumHeight, 5);
    Q_SIMPLE_BOUND_PROPERTY(ConwayCanvas, int64_t, userViewX, UserViewX, 0);
    Q_SIMPLE_BOUND_PROPERTY(ConwayCanvas, int64_t, userViewY, UserViewY, 0);
    Q_SIMPLE_BOUND_PROPERTY(ConwayCanvas, bool, enableUserViewPosition, EnableUserViewPosition, false);
    Q_SIMPLE_BOUND_PROPERTY(ConwayCanvas, bool, enableUserViewDimensions, EnableUserViewDimensions, false);

    ConwayGame::State _latestState;
    QTimer _renderTimer;
    struct GridStatistics {
        uint64_t viewWidth;
        uint64_t viewHeight;
        double cellSideLengthPx;
        // Origin of view
        double viewOriginX;
        double viewOriginY;
    };
    GridStatistics getGridStatistics();
};