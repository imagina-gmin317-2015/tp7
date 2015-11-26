#ifndef OCTREE_H
#define OCTREE_H

#include <QVector>

#include "plyloader.h"

class Octree
{
public:
    Octree();
    Octree(float x, float y, float z, float w, float h, float d, int profondeur, int limit);

    bool isEnd();
    void divide(QVector<PlyLoader* > mods);
    bool contains(PlyLoader* ply);

    float x, y, z, w, h, d = 0.0f;
    int limit = 0;
    int profondeur = 0;
    bool alreadyDivided = false;

    QVector<Octree*> fils;
    QVector<PlyLoader*> objects;
};

#endif // OCTREE_H
