#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "GraphicNode.h"

class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomGraphicsScene(const QString &name, QObject *parent = 0);
    ~CustomGraphicsScene();

signals:
    void nodeContextMenu(GraphicNode* node);
    void nodeDoubleClick(GraphicNode* node);

public slots:

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * contextMenuEvent);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
};

#endif // CUSTOMGRAPHICSSCENE_H
