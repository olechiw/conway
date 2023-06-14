#include "ConwayCanvas.h"

ConwayCanvas::ConwayCanvas(QQuickItem*)
{
    setFlag(Flag::ItemHasContents);
    connect(&_renderTimer, &QTimer::timeout, this, &ConwayCanvas::update);
    _renderTimer.setInterval(FRAME_TIME_MS);
    _renderTimer.start();
    setAcceptedMouseButtons(Qt::LeftButton);
}

ConwayCanvas::GridStatistics ConwayCanvas::getGridStatistics()
{
    GridStatistics stats{};
    uint64_t minWidth, minHeight;
    if (getEnableUserViewDimensions()) {
        // Only rendering around a single point
        minWidth = getUserViewMinimumWidth();
        minHeight = getUserViewMinimumHeight();
    }
    else {
        minWidth = (_latestState.largestXSeen) * 2 + 1;
        minHeight = (_latestState.largestYSeen) * 2 + 1;
        setUserViewMinimumWidth(minWidth);
        setUserViewMinimumHeight(minHeight);
    }
    stats.cellSideLengthPx = std::min(width() / minWidth, height() / minHeight);
    stats.viewWidth = width() / stats.cellSideLengthPx;
    stats.viewHeight = height() / stats.cellSideLengthPx;

    if (getEnableUserViewPosition()) {
        stats.viewOriginX = getUserViewX();
        stats.viewOriginY = getUserViewY();
    }
    else {
        stats.viewOriginX = 0;
        stats.viewOriginY = 0;
        setUserViewX(0);
        setUserViewY(0);
    }
    return stats;
}

// Origin is in top left
// Rectangles are positioned from top left
QSGNode* ConwayCanvas::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    _latestState = emit requestLatestState();

    QSGSimpleRectNode* parent = static_cast<QSGSimpleRectNode*>(node);
    if (!parent) {
        parent = new QSGSimpleRectNode();
        parent->setColor(Qt::black);
    }
    QRectF rect = boundingRect();
    parent->setRect(rect);


    while (parent->childCount() != 0) {
        QSGSimpleRectNode *toDelete = static_cast<QSGSimpleRectNode*>(parent->firstChild());
        parent->removeChildNode(toDelete);
        delete toDelete;
    }

    const GridStatistics grid = getGridStatistics();
    // Even can work but only if we track directions!!!!!
    const auto paddingX = uint64_t(width()) % uint64_t(grid.viewWidth * grid.cellSideLengthPx);
    const auto paddingY = uint64_t(height()) % uint64_t(grid.viewHeight * grid.cellSideLengthPx);
    const auto originOffsetX = ((grid.viewWidth / 2) * grid.cellSideLengthPx) + 1 + paddingX / 2;
    const auto originOffsetY = ((grid.viewHeight / 2) * grid.cellSideLengthPx) + 1 + paddingY / 2;

    for (const auto& [cellPosition, _] : _latestState.grid) {
        const auto renderX = (cellPosition.x - grid.viewOriginX) * grid.cellSideLengthPx + originOffsetX;
        const auto renderY = (cellPosition.y - grid.viewOriginY) * grid.cellSideLengthPx + originOffsetY;
        if (!boundingRect().contains(renderX, renderY)) continue;
        QSGSimpleRectNode* rectToRender = new QSGSimpleRectNode;
        rectToRender->setFlag(QSGNode::Flag::OwnedByParent, false);
        rectToRender->setColor(Qt::green);
        rectToRender->setRect(
            renderX,
            renderY,
            std::max(grid.cellSideLengthPx - 1, 1.),
            std::max(grid.cellSideLengthPx - 1, 1.));
        parent->appendChildNode(rectToRender);
    }


    if (getDrawGridLines() && grid.viewWidth < (width() / 3) && grid.viewHeight < (height() / 3)) {
        auto createLine = [&]() {
            QSGSimpleRectNode* rect = new QSGSimpleRectNode;
            rect->setFlag(QSGNode::Flag::OwnedByParent, false);
            rect->setColor(Qt::gray);
            parent->appendChildNode(rect);
            return rect;
        };
        for (uint64_t i = 1; i <= grid.viewWidth; ++i) {
            QSGSimpleRectNode* vertical = createLine();
            vertical->setRect(
                i * grid.cellSideLengthPx + paddingX / 2,
                0,
                1.0,
                height()
            );
        }
        for (uint64_t i = 0; i <= grid.viewHeight; ++i) {
            QSGSimpleRectNode* horizontal = createLine();
            horizontal->setRect(
                0,
                i * grid.cellSideLengthPx + paddingY / 2,
                width(),
                1.0
            );
        }
    }

    return parent;
}

void ConwayCanvas::mousePressEvent(QMouseEvent* event)
{
    GridStatistics grid = getGridStatistics();
    const int64_t x = int64_t(event->pos().x() / grid.cellSideLengthPx) - int64_t(grid.viewWidth / 2) + grid.viewOriginX;
    const int64_t y = int64_t(event->pos().y() / grid.cellSideLengthPx) - int64_t(grid.viewHeight / 2) + grid.viewOriginY;
    emit onClicked(x, y);
}