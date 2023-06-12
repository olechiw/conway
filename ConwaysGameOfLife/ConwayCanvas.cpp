#include "ConwayCanvas.h"

ConwayCanvas::ConwayCanvas(QQuickItem* parent)
{
    setFlag(Flag::ItemHasContents);
    connect(&_renderTimer, &QTimer::timeout, this, &ConwayCanvas::update);
    _renderTimer.setInterval(FRAME_TIME_MS);
    _renderTimer.start();
    setAcceptedMouseButtons(Qt::LeftButton);
}

static void drawGridLines(QSGSimpleRectNode* parent, long gameSize, double cellWidth, double cellHeight) {
    auto createLine = [&]() {
        QSGSimpleRectNode* rect = new QSGSimpleRectNode;
        rect->setFlag(QSGNode::Flag::OwnedByParent, false);
        rect->setColor(Qt::gray);
        parent->appendChildNode(rect);
        return rect;
    };
    for (long i = 1; i < gameSize; ++i) {
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

// Origin is in top left
// Rectangles are positioned from top left
struct GridStatistics {
    const long gridSize; // Width + Height, grid is always a square (for now)
    const double cellWidth;
    const double cellHeight;
    // Offsets for top-left corner positioning
    const double originOffsetX;
    const double originOffsetY;

    GridStatistics(const double &canvasWidth, const double &canvasHeight, const long &gridSize): 
        gridSize(gridSize),
        cellWidth(canvasWidth / gridSize),
        cellHeight(canvasHeight / gridSize),
        originOffsetX(canvasWidth / 2 - cellWidth / 2 + 1),
        originOffsetY(canvasHeight / 2 - cellHeight / 2 + 1)
    {
    }
};

QSGNode* ConwayCanvas::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    QSGSimpleRectNode* n = static_cast<QSGSimpleRectNode*>(node);
    if (!n) {
        n = new QSGSimpleRectNode();
        n->setColor(Qt::black);
    }
    QRectF rect = boundingRect();
    n->setRect(rect);

    if (_currentState.size > 0) {
        while (n->childCount() != 0) {
            QSGSimpleRectNode *toDelete = static_cast<QSGSimpleRectNode*>(n->firstChild());
            n->removeChildNode(toDelete);
            delete toDelete;
        }

        const GridStatistics grid(width(), height(), _currentState.size * 2 + 1);

        for (const auto& [cellPosition, _] : _currentState.board) {
            QSGSimpleRectNode* rectToRender = new QSGSimpleRectNode;
            rectToRender->setFlag(QSGNode::Flag::OwnedByParent, false);
            rectToRender->setColor(Qt::green);
            rectToRender->setRect(
                cellPosition.x * grid.cellWidth + grid.originOffsetX,
                cellPosition.y * grid.cellHeight + grid.originOffsetY,
                std::max(grid.cellWidth - 1, 1.),
                std::max(grid.cellHeight - 1, 1.));
            n->appendChildNode(rectToRender);
        }
        if (getDrawGridLines() && grid.gridSize < (boundingRect().width() / 3)) {
            drawGridLines(n, grid.gridSize, grid.cellWidth, grid.cellHeight);
        }
    }

    return n;
}

void ConwayCanvas::mousePressEvent(QMouseEvent* event)
{
    GridStatistics grid(width(), height(), _currentState.size * 2 + 1);
    const long x = long(event->pos().x() / grid.cellWidth) - grid.gridSize / 2;
    const long y = long(event->pos().y() / grid.cellHeight) - grid.gridSize / 2;
    emit onClicked(x, y);
}

void ConwayCanvas::gameStateChanged(const ConwayGame::State& state)
{
    _currentState = state;
}