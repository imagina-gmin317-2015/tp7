#include "sphere.h"

#include "math.h"

#include <QString>
#include <QDebug>

Sphere::Sphere()
{
}

Sphere::Sphere(float x, float y, float z, float radius)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->radius = radius;
}

void Sphere::display()
{
    qDebug()<<this->x << "/" << this->y << "/" << this->z << " : " << this->radius;
}

bool Sphere::hasCollision(Sphere *s)
{
    return (sqrt(pow(this->x - s->x, 2) + pow(this->y - s->y, 2) + pow(this->z - s->z, 2)) <= this->radius + s->radius);
}

bool Sphere::hasCollision(AABB *p)
{
    if(p->isIn(this->x, this->y, this->z)){
        return true;
    }

    if(p->isIn(this->x + this->radius, this->y, this->z) || p->isIn(this->x - this->radius, this->y, this->z) || p->isIn(this->x, this->y + this->radius, this->z) || p->isIn(this->x, this->y - this->radius, this->z) || p->isIn(this->x, this->y, this->z + this->radius) || p->isIn(this->x, this->y, this->z - this->radius)){
        return true;
    }
    return false;
}
