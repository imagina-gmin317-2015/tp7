#include "aabb.h"

#include <QDebug>

AABB::AABB()
{

}

AABB::AABB(float x, float y, float z, float w, float h, float d)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
    this->h = h;
    this->d = d;
}

bool AABB::hasCollision(AABB *b)
{
    return this->x <= b->x + b->w
                && this->x + this->w >= b->x
                && this->y <= b->y + b->h
                && this->y + this->h >= b->y
                && this->z <= b->z + b->d
            && this->z + this->d >= b->z;
}

bool AABB::isIn(float x, float y, float z)
{
    return this->x <= x && this->x + this->w >= x
            && this->y <= y && this->y + this->h >= y
            && this->z <= z && this->z + this->d >= z;
}

void AABB::display()
{
    qDebug()<<this->x << "/" << this->y << "/" << this->z << " : " << this->x + this->w << "/" << this->y + this->h << "/" << this->z + this->d;
}
