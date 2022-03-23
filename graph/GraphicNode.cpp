// GraphicNode.cpp

#include "GraphicNode.h"
#include <iostream>

GraphicNode::GraphicNode(qreal x, qreal y, qreal radius) {
    this->x = x;
    this->y = y;
    this->radius = radius;
}

QRectF GraphicNode::boundingRect() const
{
    // outer most edges
    return QRectF(this->x,this->y,this->radius * 2,this->radius * 2); // edit with radius later
}

void GraphicNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = boundingRect();
    QPen pen(Qt::black, 3);
    painter->setBrush(Qt::black);
    // painter->setPen(pen);
    painter->drawEllipse(rect);
    // painter->fillRect(rect, QBrush(Qt::black));
}

void GraphicNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = true;
    update();
    // add zoom in logic soon
    QGraphicsItem::mousePressEvent(event);
}

void GraphicNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
