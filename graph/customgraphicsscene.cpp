#include "customgraphicsscene.h"
#include <iostream>

CustomGraphicsScene::CustomGraphicsScene(const QString &name, QObject *parent) : QGraphicsScene(parent)
{
  // TODO ADD SOME STUFF
}

CustomGraphicsScene::~CustomGraphicsScene()
{
}

#include <QGraphicsSceneContextMenuEvent>
void CustomGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    QGraphicsItem *item = itemAt(contextMenuEvent->scenePos(), QTransform());
    if(item)
    {
        item->setSelected(true);
        if(item->type() == GraphicNode::Type)
            emit nodeContextMenu(qgraphicsitem_cast<GraphicNode*>(item));
    }
    QGraphicsScene::contextMenuEvent(contextMenuEvent);
}

void CustomGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
    if(item)
    {
        if(item->type() == GraphicNode::Type) {
            std::cerr << "WELCOME!";
            emit nodeDoubleClick(qgraphicsitem_cast<GraphicNode*>(item));
        }
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}
