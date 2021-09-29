#ifndef QGVNODE_H
#define QGVNODE_H

#include "qgv.h"
#include <QGraphicsItem>
#include <QPen>

class QGVEdge;
class QGVScene;
class QGVNodePrivate;

/**
 * @brief Node item
 *
 */
class QGVCORE_EXPORT QGVNode : public QGraphicsItem
{
public:
    ~QGVNode();

    QString label() const;
    void setLabel(const QString &label);

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setAttribute(const QString &label, const QString &value);
    QString getAttribute(const QString &name) const;

    void setIcon(const QImage &icon);

    enum { Type = UserType + 2 };
    int type() const
    {
        return Type;
    }

private:
    friend class QGVScene;
    friend class QGVSubGraph;
    void updateLayout();
        QGVNode(QGVNodePrivate* node, QGVScene *scene);

        // Not implemented in QGVNode.cpp
//		QPainterPath makeShape(Agnode_t* node) const;
//		QPolygonF makeShapeHelper(Agnode_t* node) const;

    QPainterPath _path;
    QPen _pen;
    QBrush _brush;
    QImage _icon;

    QGVScene *_scene;
        QGVNodePrivate* _node;
};


#endif // QGVNODE_H
