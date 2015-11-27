#ifndef OCTREE_H
#define OCTREE_H

#include <QSet>
#include <QVector>
#include <QtGui/QOpenGLFunctions>

#include "plyentity.h"

#define MIN_SIZE 1.0 / 32.0

class Node {
public:
    Node();
    Node(float x, float y, float z, float width, float height, float depth);
    void addEntity(PlyEntity *entity);
    void draw();
    void clear();
    bool equal(Node n);

private:
    float width, height, depth, x, y, z;
    QSet<PlyEntity *> entities;
    bool collides;

    //0-3 bas
    //4-7 haut
    //paire gauche
    //impaire droite
    QVector<Node> nodes;
};

class Octree
{
public:
    Octree();

    QSet<PlyEntity *> entities;

    void addEntity(PlyEntity *entity);
    void draw();
    void update();

private:
    Node parent;
};

#endif // OCTREE_H
