#include "ConwayCanvas.h"

ConwayCanvas::ConwayCanvas(QQuickItem*)
{
    setFlag(Flag::ItemHasContents);
    connect(&_renderTimer, &QTimer::timeout, this, &ConwayCanvas::update);
    _renderTimer.setInterval(FRAME_TIME_MS);
    _renderTimer.start();
    setAcceptedMouseButtons(Qt::LeftButton);
}

static void drawGridLines(QSGSimpleRectNode* parent, uint64_t gameSize, double cellWidth, double cellHeight) {
    auto createLine = [&]() {
        QSGSimpleRectNode* rect = new QSGSimpleRectNode;
        rect->setFlag(QSGNode::Flag::OwnedByParent, false);
        rect->setColor(Qt::gray);
        parent->appendChildNode(rect);
        return rect;
    };
    for (uint64_t i = 1; i < gameSize; ++i) {
        QSGSimpleRectNode* vertical = createLine();
        vertical->setRect(
            i * cellWidth,
            0,
            1.0,
            parent->rect().height()
        );

        QSGSimpleRectNode* horizontal = createLine();
        horizontal->setRect(
            0,
            i * cellHeight,
            parent->rect().width(),
            1.0
        );
    }
}

ConwayCanvas::GridStatistics ConwayCanvas::getGridStatistics()
{
    GridStatistics stats{};
    stats.gridSize = std::max(_latestState.largestXSeen, _latestState.largestYSeen) * 2 + 1;
    if (getEnableUserView()) {
        //stats.gridSize = getUserViewSize(); (unsupported for now)
        stats.originX = getUserViewX();
        stats.originY = getUserViewY();
    }
    else {
        stats.originX = 0;
        stats.originY = 0;
    }
    stats.cellWidth = width() / stats.gridSize;
    stats.cellHeight = height() / stats.gridSize;
    return stats;
}

// Origin is in top left
// Rectangles are positioned from top left
QSGNode* ConwayCanvas::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    _latestState = emit requestLatestState();

    QSGSimpleRectNode* n = static_cast<QSGSimpleRectNode*>(node);
    if (!n) {
        n = new QSGSimpleRectNode();
        n->setColor(Qt::black);
    }
    QRectF rect = boundingRect();
    n->setRect(rect);

    while (n->childCount() != 0) {
        QSGSimpleRectNode *toDelete = static_cast<QSGSimpleRectNode*>(n->firstChild());
        n->removeChildNode(toDelete);
        delete toDelete;
    }

    const GridStatistics grid = getGridStatistics();
    const auto originOffsetX = (width() / 2 - grid.cellWidth / 2 + 1);
    const auto originOffsetY = (height() / 2 - grid.cellHeight / 2 + 1);

    for (const auto& [cellPosition, _] : _latestState.grid) {
        QSGSimpleRectNode* rectToRender = new QSGSimpleRectNode;
        rectToRender->setFlag(QSGNode::Flag::OwnedByParent, false);
        rectToRender->setColor(Qt::green);
        rectToRender->setRect(
            (cellPosition.x - grid.originX) * grid.cellWidth + originOffsetX,
            (cellPosition.y - grid.originY) * grid.cellHeight + originOffsetY,
            std::max(grid.cellWidth - 1, 1.),
            std::max(grid.cellHeight - 1, 1.));
        n->appendChildNode(rectToRender);
    }
    if (getDrawGridLines() && grid.gridSize < (boundingRect().width() / 3)) {
        drawGridLines(n, grid.gridSize, grid.cellWidth, grid.cellHeight);
    }

    return n;
}

void ConwayCanvas::mousePressEvent(QMouseEvent* event)
{
    GridStatistics grid = getGridStatistics();
    const int64_t x = int64_t(event->pos().x() / grid.cellWidth) - int64_t(grid.gridSize / 2) - grid.originX;
    const int64_t y = int64_t(event->pos().y() / grid.cellHeight) - int64_t(grid.gridSize / 2) - grid.originY;
    emit onClicked(x, y);
}