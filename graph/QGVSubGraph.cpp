#include "QGVSubGraph.h"
#include "private/QGVCore.h"
#include "QGVScene.h"
#include "private/QGVGraphPrivate.h"
#include "private/QGVNodePrivate.h"
#include "QGVNode.h"
#include <QDebug>
#include <QPainter>

QGVSubGraph::QGVSubGraph(QGVGraphPrivate *subGraph, QGVScene *scene): _sgraph(subGraph), _scene(scene)
{
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QGVSubGraph::~QGVSubGraph()
{
    _scene->removeItem(this);
        delete _sgraph;
}

QString QGVSubGraph::name() const
{
        return QString::fromLocal8Bit(GD_label(_sgraph->graph())->text);
}

QGVNode *QGVSubGraph::addNode(const QString &label)
{
        Agnode_t *node = agnode(_sgraph->graph(), NULL, TRUE);
    if(node == NULL)
    {
        qWarning()<<"Invalid sub node :"<<label;
        return 0;
    }
        agsubnode(_sgraph->graph(), node, TRUE);

        QGVNode *item = new QGVNode(new QGVNodePrivate(node), _scene);
    item->setLabel(label);
    _scene->addItem(item);
    _scene->_nodes.append(item);
    _nodes.append(item);
    return item;
}

QGVSubGraph *QGVSubGraph::addSubGraph(const QString &name, bool cluster)
{
    Agraph_t* sgraph;
    if(cluster)
                sgraph = agsubg(_sgraph->graph(), ("cluster_" + name).toLocal8Bit().data(), TRUE);
    else
                sgraph = agsubg(_sgraph->graph(), name.toLocal8Bit().data(), TRUE);

    if(sgraph == NULL)
    {
        qWarning()<<"Invalid subGraph :"<<name;
        return 0;
    }

        QGVSubGraph *item = new QGVSubGraph(new QGVGraphPrivate(sgraph), _scene);
    _scene->_subGraphs.append(item);
    _scene->addItem(item);
    return item;
}

QRectF QGVSubGraph::boundingRect() const
{
    return QRectF(0,0, _width, _height);
}

void QGVSubGraph::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();

    painter->setPen(_pen);
    painter->setBrush(_brush);

    painter->drawRect(boundingRect());
    painter->drawText(_label_rect, Qt::AlignCenter, _label);
    painter->restore();
}

void QGVSubGraph::setAttribute(const QString &name, const QString &value)
{
        agsafeset(_sgraph->graph(), name.toLocal8Bit().data(), value.toLocal8Bit().data(), "");
}

QString QGVSubGraph::getAttribute(const QString &name) const
{
        char* value = agget(_sgraph->graph(), name.toLocal8Bit().data());
    if(value)
        return value;
    return QString();
}

void QGVSubGraph::updateLayout()
{
    prepareGeometryChange();

    //SubGraph box
        boxf box = GD_bb(_sgraph->graph());
    pointf p1 = box.UR;
    pointf p2 = box.LL;
    _width = p1.x - p2.x;
    _height = p1.y - p2.y;

        qreal gheight = QGVCore::graphHeight(_scene->_graph->graph());
    setPos(p2.x, gheight - p1.y);

    _pen.setWidth(1);
    _brush.setStyle(QGVCore::toBrushStyle(getAttribute("style")));
    _brush.setColor(QGVCore::toColor(getAttribute("fillcolor")));
    _pen.setColor(QGVCore::toColor(getAttribute("color")));

    //SubGraph label
        textlabel_t *xlabel = GD_label(_sgraph->graph());
    if(xlabel)
    {
        _label = xlabel->text;
        _label_rect.setSize(QSize(xlabel->dimen.x, xlabel->dimen.y));
                _label_rect.moveCenter(QGVCore::toPoint(xlabel->pos, QGVCore::graphHeight(_scene->_graph->graph())) - pos());
    }
}
