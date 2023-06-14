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
        minWidth = getUserViewMinimumWidth();
        minHeight = getUserViewMinimumHeight();
    }
    else {
        minWidth = (_latestState.largestXSeen) * 2 + 1;
        minHeight = (_latestState.largestYSeen) * 2 + 1;
        // TODO: move to frontend
        setUserViewMinimumWidth(minWidth);
        setUserViewMinimumHeight(minHeight);
    }
    stats.cellSideLengthPx = std::min(width() / minWidth, height() / minHeight);
    stats.viewWidth = width() / stats.cellSideLengthPx;
    stats.viewHeight = height() / stats.cellSideLengthPx;

    if (!getEnableUserViewDimensions()) {
        // Autosizing looks smoother when always using odd width + height

        // width and height are constants
        // minWidth and minHeight are guaranteed to be odd
        // L = max(minWidth, minHeight), S = min(minWidth, minHeight)
        // Smallest side length allowed, X = (width or height respectively) / S
        // L2 = (width or height respectively) / X = the number of cells that fit in the less cramped direction
        // L2 >= L trivially because we picked size based on the more cramped direction
        // if L2 is even, we can subtract one because we know L2 >= L and L is odd\
        // We don't have to track which one is L2 because S cannot be even
        if (stats.viewWidth % 2 == 0) stats.viewWidth--;
        if (stats.viewHeight % 2 == 0) stats.viewHeight--;
    }

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
    stats.paddingXPx = width() - stats.viewWidth * stats.cellSideLengthPx;
    stats.paddingYPx = height() - stats.viewHeight * stats.cellSideLengthPx;

    // stats.viewWidth / 2 and stats.viewHeight / 2 will give the origin's position in view-space
    // We shift the origin by 1px to leave space for a border
    // TODO: borders are not actually pixel perfect
    stats.viewOriginXPx = ((stats.viewWidth / 2) * stats.cellSideLengthPx) + 1 + stats.paddingXPx / 2;
    stats.viewOriginYPx = ((stats.viewHeight / 2) * stats.cellSideLengthPx) + 1 + stats.paddingYPx / 2;
    return stats;
}

void ConwayCanvas::renderLatestGrid(QSGNode* parent)
{
    const GridStatistics grid = getGridStatistics();

    for (const auto& [x, y] : _latestState.grid) {
        const auto renderX = (x - grid.viewOriginX) * grid.cellSideLengthPx + grid.viewOriginXPx;
        const auto renderY = (y - grid.viewOriginY) * grid.cellSideLengthPx + grid.viewOriginYPx;
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
}

void ConwayCanvas::renderGridLines(QSGNode* parent)
{
    const GridStatistics grid = getGridStatistics();

    const bool viewTooLarge = (grid.viewWidth >= (width() / 3) || grid.viewHeight >= (height() / 3));
    if (getDrawGridLines() && !viewTooLarge) {
        auto createLine = [&]() {
            QSGSimpleRectNode* rect = new QSGSimpleRectNode;
            rect->setFlag(QSGNode::Flag::OwnedByParent, false);
            rect->setColor(Qt::gray);
            parent->appendChildNode(rect);
            return rect;
        };
        // Verticle line at position 0 looks strange
        const bool skipFirstVertLine = (grid.paddingXPx == 0);
        for (uint64_t i = skipFirstVertLine; i <= grid.viewWidth; ++i) {
            QSGSimpleRectNode* vertical = createLine();
            vertical->setRect(
                i * grid.cellSideLengthPx + grid.paddingXPx / 2,
                0,
                1.0,
                height()
            );
        }
        for (uint64_t i = 0; i <= grid.viewHeight; ++i) {
            QSGSimpleRectNode* horizontal = createLine();
            horizontal->setRect(
                0,
                i * grid.cellSideLengthPx + grid.paddingYPx / 2,
                width(),
                1.0
            );
        }
    }
}

QSGNode* ConwayCanvas::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    _latestState = emit requestLatestState();

    QSGSimpleRectNode* parent = static_cast<QSGSimpleRectNode*>(node);
    if (!parent) {
        parent = new QSGSimpleRectNode();
        parent->setColor(Qt::black);
    }
    parent->setRect(boundingRect());

    while (parent->childCount() != 0) {
        QSGSimpleRectNode *toDelete = static_cast<QSGSimpleRectNode*>(parent->firstChild());
        parent->removeChildNode(toDelete);
        delete toDelete;
    }

    renderLatestGrid(parent);
    renderGridLines(parent);

    return parent;
}

void ConwayCanvas::mousePressEvent(QMouseEvent* event)
{
    GridStatistics grid = getGridStatistics();
    auto xFrac = (event->pos().x() - grid.viewOriginXPx) / grid.cellSideLengthPx;
    auto yFrac = (event->pos().y() - grid.viewOriginYPx) / grid.cellSideLengthPx;
    // Fractions are from top-left corner so round up negative numbers
    const auto round = [](const auto val) {
        if (val < 0)
            return int64_t(val - 1);
        else
            return int64_t(val);
    };
    emit onClicked(round(xFrac), round(yFrac));
}