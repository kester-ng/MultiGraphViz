#ifndef QGVSUBGRAPH_H
#define QGVSUBGRAPH_H

#include "qgv.h"
#include <QGraphicsItem>
#include <QPen>

class QGVNode;
class QGVEdge;
class QGVScene;
class QGVGraphPrivate;

/**
 * @brief SubGraph item
 *
 */
class QGVCORE_EXPORT QGVSubGraph : public QGraphicsItem
{
public:
    ~QGVSubGraph();

    QString name() const;

    QGVNode* addNode(const QString& label);
    QGVSubGraph* addSubGraph(const QString& name, bool cluster=true);

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setAttribute(const QString &name, const QString &value);
    QString getAttribute(const QString &name) const;
    void updateLayout();

    enum { Type = UserType + 4 };
    int type() const
    {
        return Type;
    }


private:
    friend class QGVScene;
        QGVSubGraph(QGVGraphPrivate* subGraph, QGVScene *scene);

    double _height, _width;
    QPen _pen;
    QBrush _brush;

    QString _label;
    QRectF _label_rect;

    QGVScene *_scene;
        QGVGraphPrivate *_sgraph;
    QList<QGVNode*> _nodes;
};

#endif // QGVSUBGRAPH_H
