#ifndef QGVEDGE_H
#define QGVEDGE_H

#include "qgv.h"
#include <QGraphicsItem>
#include <QPen>

class QGVNode;
class QGVScene;
class QGVEdgePrivate;

/**
 * @brief Edge item
 *
 */
class QGVCORE_EXPORT QGVEdge : public QGraphicsItem
{
public:
    ~QGVEdge();

    QString label() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void setLabel(const QString &label);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void setAttribute(const QString &name, const QString &value);
    QString getAttribute(const QString &name) const;

    void updateLayout();

    enum { Type = UserType + 3 };
    int type() const
    {
        return Type;
    }

private:
        QGVEdge(QGVEdgePrivate *edge, QGVScene *scene);

    QPolygonF toArrow(const QLineF &normal) const;

    friend class QGVScene;
    //friend class QGVSubGraph;

    QGVScene *_scene;
        QGVEdgePrivate* _edge;

    QPainterPath _path;
    QPen _pen;
    QPolygonF _head_arrow;
    QPolygonF _tail_arrow;

    QString _label;
    QRectF _label_rect;
};

#endif // QGVEDGE_H
