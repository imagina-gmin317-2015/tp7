#include "qquadtree.h"

QQuadTree::QQuadTree(Terrain* terrain, QQuadTree *parent) : QObject(parent), QuadTree(terrain)
{

}


QQuadTree* QQuadTree::parent() {

    return dynamic_cast<QQuadTree*>(QObject::parent());
}

void QQuadTree::makeChildren()
{

}

QList<QQuadTree> QQuadTree::getTopNeighbours()
{
    return QList<QQuadTree>();

}

QList<QQuadTree> QQuadTree::getBottomNeighbours()
{
    return QList<QQuadTree>();

}

QList<QQuadTree> QQuadTree::getLeftNeighbours()
{
    return QList<QQuadTree>();

}

QList<QQuadTree> QQuadTree::getRightNeighbours()
{
    return QList<QQuadTree>();

}

bool QQuadTree::sameWidth(QQuadTree *neighbour)
{
    return false;

}

bool QQuadTree::sameHeight(QQuadTree *neighbour)
{
    return false;

}

bool QQuadTree::haveTopNeighbour(QQuadTree *neighbour)
{
    return false;
}

void QQuadTree::setParent(QQuadTree *parent) {
    QObject::setParent(parent);
}
