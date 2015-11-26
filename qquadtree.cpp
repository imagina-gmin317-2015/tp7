#include "qquadtree.h"

QQuadTree::QQuadTree(Terrain* terrain, QQuadTree *parent) : QObject(parent), QuadTree(terrain)
{

}


QQuadTree* QQuadTree::parent() {

    return dynamic_cast<QQuadTree*>(QObject::parent());
}

void QQuadTree::setParent(QQuadTree *parent) {
    QObject::setParent(parent);
}
