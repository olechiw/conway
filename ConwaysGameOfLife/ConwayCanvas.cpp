#include "ConwayCanvas.h"

ConwayCanvas::ConwayCanvas(QQuickItem* parent)
{
    setFlag(Flag::ItemHasContents);
    _currentState.size = 0;
}

QSGNode* ConwayCanvas::updatePaintNode(QSGNode* node, UpdatePaintNodeData*)
{
    QSGSimpleRectNode* n = static_cast<QSGSimpleRectNode*>(node);
    if (!n) {
        n = new QSGSimpleRectNode();
        n->setColor(Qt::black);
        QRectF rect = boundingRect();
        n->setRect(rect);
    }

    if (_currentState.size > 0) {
        while (n->childCount() != 0) {
            QSGSimpleRectNode *toDelete = static_cast<QSGSimpleRectNode*>(n->firstChild());
            n->removeChildNode(toDelete);
            delete toDelete;
        }

        const long gameSize = _currentState.size;
        const auto rectangleWidth = this->width() / (gameSize * 2);
        const auto rectangleHeight = this->height() / (gameSize * 2);
        const auto xOffset = (this->width() / 2) - (rectangleWidth / 2);
        const auto yOffset = (this->height() / 2) - (rectangleHeight / 2);

        for (const auto& [cellPosition, _] : _currentState.board) {
            QSGSimpleRectNode* rectToRender = new QSGSimpleRectNode;
            rectToRender = new(rectToRender) QSGSimpleRectNode;
            rectToRender->setFlag(QSGNode::Flag::OwnedByParent, false);
            rectToRender->setColor(Qt::green);
            rectToRender->setRect(
                cellPosition.x * rectangleWidth + xOffset,
                cellPosition.y * rectangleHeight + yOffset,
                std::max(rectangleWidth - 1, 1.),
                std::max(rectangleHeight - 1, 1.));
            n->appendChildNode(rectToRender);
        }
    }

    return n;
}

void ConwayCanvas::nextGameState(const ConwayGame::State& state)
{
    _currentState = state;
    emit update();
}