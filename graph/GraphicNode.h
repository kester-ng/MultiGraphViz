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
    std::string name;
    /*
    GraphicNode(qreal x, qreal y, qreal radius) {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }*/
    GraphicNode(qreal x, qreal y, qreal radius, std::string name);
    QRectF boundingRect() const;

    // overriding paint()
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    // item state
    bool Pressed;
    enum { Type = UserType + 2 };
    int type() const
    {
        return Type;
    }
protected:
    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // GRAPHICNODE_H
