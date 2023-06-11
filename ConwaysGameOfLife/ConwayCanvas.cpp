#include "ConwayCanvas.h"

ConwayCanvas::ConwayCanvas(QQuickItem* parent)
{
    setFlag(Flag::ItemHasContents);
    _currentState.size = 0;
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

        const long gameSize = (_currentState.size) * 2 + 1;
        const auto rectangleWidth = this->width() / gameSize;
        const auto rectangleHeight = this->height() / gameSize;
        const auto xOffset = (this->width() / 2) - (rectangleWidth / 2) + 1;
        const auto yOffset = (this->height() / 2) - (rectangleHeight / 2) + 1;

        for (const auto& [cellPosition, _] : _currentState.board) {
            QSGSimpleRectNode* rectToRender = new QSGSimpleRectNode;
            rectToRender->setFlag(QSGNode::Flag::OwnedByParent, false);
            rectToRender->setColor(Qt::green);
            rectToRender->setRect(
                cellPosition.x * rectangleWidth + xOffset,
                cellPosition.y * rectangleHeight + yOffset,
                std::max(rectangleWidth - 1, 1.),
                std::max(rectangleHeight - 1, 1.));
            n->appendChildNode(rectToRender);
        }
        if (getDrawGridLines() && gameSize < boundingRect().width() / 3)
            drawGridLines(n, gameSize, rectangleWidth, rectangleHeight);
    }

    return n;
}

void ConwayCanvas::nextGameState(const ConwayGame::State& state)
{
    _currentState = state;
}