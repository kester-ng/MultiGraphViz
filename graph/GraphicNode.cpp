// GraphicNode.cpp

#include "GraphicNode.h"

GraphicNode::GraphicNode(qreal x, qreal y, qreal radius) {
    this->x = x;
    this->y = y;
    this->radius = radius;
}

QRectF GraphicNode::boundingRect() const
{
    // outer most edges
    return QRectF(0,0,100,100); // edit with radius later
}

void GraphicNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();

    if(Pressed)
    {
        QPen pen(Qt::red, 3);
        painter->setPen(pen);
        painter->drawEllipse(rect);
    }
    else
    {
        QPen pen(Qt::black, 3);
        painter->setPen(pen);
        painter->drawRect(rect);
    }

    this->setX(this->x);
    this->setY(this->y);
}

void GraphicNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void GraphicNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
