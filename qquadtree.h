#ifndef QQUADTREE_H
#define QQUADTREE_H

#include "terrain.h"
#include "quadtree.h"
#include <QObject>

class QQuadTree : private virtual QObject, public virtual QuadTree
{
    Q_OBJECT
public:
    explicit QQuadTree(Terrain* terrain, QQuadTree *parent = 0);
    virtual void setParent(QQuadTree* parent);
    virtual QQuadTree* parent();
    virtual void makeChildren();
    virtual QList<QQuadTree> getTopNeighbours();
    virtual QList<QQuadTree> getBottomNeighbours();
    virtual QList<QQuadTree> getLeftNeighbours();
    virtual QList<QQuadTree> getRightNeighbours();
protected:
    virtual bool sameWidth(QQuadTree* neighbour);
    virtual bool sameHeight(QQuadTree* neighbour);
    virtual bool haveTopNeighbour(QQuadTree* neighbour);
signals:

public slots:
};

#endif // QQUADTREE_H
