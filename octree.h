#ifndef OCTREE_H
#define OCTREE_H

#include <QVector>

#include "plyloader.h"

class Octree
{
public:
    Octree();
    Octree(float x, float y, float z, float w, float h, float d, int profondeur, int limit, QVector<PlyLoader* > mods);

    bool isEnd();
    void divide();
    bool contains(PlyLoader* ply);
    bool display();

    float x, y, z, w, h, d = 0.0f;
    int limit = 0;
    int profondeur = 0;
    bool alreadyDivided = false;

    QVector<Octree*> fils;
    QVector<PlyLoader*> objects;
    QVector<PlyLoader*> mods;
};

#endif // OCTREE_H
