#ifndef GRAPHICNODE_H
#define GRAPHICNODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>
#include <QPolygonF>
#include <QPainterPath>
#include <QColor>

// class for customization
class GraphicNode : public QGraphicsItem
{
public:
    qreal x;
    qreal y;
    qreal radius;
    /*
    GraphicNode(qreal x, qreal y, qreal radius) {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }*/
    GraphicNode(qreal x, qreal y, qreal radius);
    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    // item state
    bool Pressed;
protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // GRAPHICNODE_H