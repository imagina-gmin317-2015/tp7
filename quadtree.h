#ifndef QUADTREE_H
#define QUADTREE_H

#include <QVector>
#include "plyloader.h"


class Quadtree
{
public:
    Quadtree();
    Quadtree(float xMin, float yMin, float xMax, float yMax, QVector<PlyLoader *> mods, int prof);
    bool contient(PlyLoader* ply); // retourne vrai si ply touche le quadtree
    void divise(float xMin, float yMin, float xMax, float yMax, QVector<PlyLoader *> mods, int prof); // cré les quatres fils du quadtree
    void affiche();
    void getPly();

    float xMin,yMin,xMax,yMax; // coordonées du rectangle
    Quadtree *fNO, *fSO, *fSE, *fNE; // les quatres fils
    QVector<PlyLoader*> object;
    int profondeur = 0;
    bool isFeuille =  false;
    bool isNull = false;



signals:

public slots:
};

#endif // QUADTREE_H
