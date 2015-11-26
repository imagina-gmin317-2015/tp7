#include "octree.h"
#include "aabb.h"

#include <QDebug>

Octree::Octree()
{
}

Octree::Octree(float x, float y, float z, float w, float h, float d, int profondeur, int limit, QVector<PlyLoader* > mods)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
    this->h = h;
    this->d = d;
    this->profondeur = profondeur;
    this->limit = limit;
    this->mods = mods;
    this->divide();

}

bool Octree::isEnd()
{
    return (this->profondeur == this->limit);
}

void Octree::divide()
{

    bool toDivide = false;
    for(int i = 0; i < this->mods.size(); i++){
        if(this->contains(this->mods.at(i))){
            this->objects.push_back(this->mods.at(i));
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
                    this->fils.push_back(new Octree(i*nw, j*nh, k*nd, nw, nh, nd, this->profondeur + 1, this->limit, this->mods));
                }
            }
        }
    }
}

bool Octree::contains(PlyLoader *ply)
{
    AABB *oc = new AABB(this->x, this->y, this->z, this->w, this->h, this->d);
    return oc->hasCollision(ply->getBounding());
}

bool Octree::display()
{
    glPointSize(2.0f);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(this->x,this->y, this->z);
    glVertex3f(this->x,this->y + this->h, this->z);
    glVertex3f(this->x + this->w,this->y + this->h, this->z);
    glVertex3f(this->x + this->w,this->y, this->z);
    glVertex3f(this->x,this->y, this->z);

    glVertex3f(this->x,this->y, this->z + this->d);
    glVertex3f(this->x,this->y + this->h, this->z + this->d);
    glVertex3f(this->x + this->w,this->y + this->h, this->z + this->d);
    glVertex3f(this->x + this->w,this->y, this->z + this->d);
    glVertex3f(this->x,this->y, this->z + this->d);

    glVertex3f(this->x,this->y, this->z);
    glVertex3f(this->x,this->y + this->h, this->z);
    glVertex3f(this->x,this->y + this->h, this->z + this->d);
    glVertex3f(this->x,this->y, this->z + this->d);
    glVertex3f(this->x,this->y, this->z);

    glVertex3f(this->x + this->w,this->y, this->z);
    glVertex3f(this->x + this->w,this->y + this->h, this->z);
    glVertex3f(this->x + this->w,this->y + this->h, this->z + this->d);
    glVertex3f(this->x + this->w,this->y, this->z + this->d);
    glVertex3f(this->x + this->w,this->y, this->z);

    glVertex3f(this->x,this->y, this->z);
    glVertex3f(this->x + this->w,this->y, this->z);
    glVertex3f(this->x + this->w,this->y, this->z + this->d);
    glVertex3f(this->x,this->y, this->z + this->d);
    glVertex3f(this->x,this->y, this->z);

    glVertex3f(this->x,this->y + this->h, this->z);
    glVertex3f(this->x + this->w,this->y + this->h, this->z);
    glVertex3f(this->x + this->w,this->y + this->h, this->z + this->d);
    glVertex3f(this->x,this->y + this->h, this->z + this->d);
    glVertex3f(this->x,this->y + this->h, this->z);

    glEnd();
    glColor3f(0.5,0.5,0.5);
    glDisable(GL_COLOR_MATERIAL);

    for(int i = 0; i < this->fils.size(); i++){
        this->fils.at(i)->display();
    }
}

