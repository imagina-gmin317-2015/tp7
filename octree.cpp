#include "octree.h"
#include "aabb.h"

Octree::Octree()
{
}

Octree::Octree(float x, float y, float z, float w, float h, float d, int profondeur, int limit)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
    this->h = h;
    this->d = d;
    this->profondeur = profondeur;
    this->limit = limit;
}

bool Octree::isEnd()
{
    return (this->profondeur == this->limit);
}

void Octree::divide(QVector<PlyLoader* > mods)
{
    bool toDivide = false;
    for(int i = 0; i < mods.size(); i++){
        if(this->contains(mods.at(i))){
            this->objects.push_back(mods.at(i));
            toDivide = true;
        }
    }

    if(toDivide && !this->isEnd()){
        float nw = this->w/2;
        float nh = this->h/2;
        float nd = this->d/2;
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                for(int k = 0; k < 2; k++){
                    this->fils.push_back(new Octree(i*nw, j*nh, k*nd, nw, nh, nd, this->profondeur + 1, this->limit));
                }
            }
        }
    }

}

bool Octree::contains(PlyLoader *ply)
{
    AABB oc = new AABB(this->x, this->y, this->z, this->w, this->h, this->d);
    return oc.hasCollision(ply->getBounding());
}

