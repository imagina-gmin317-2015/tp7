#ifndef SPHERE_H
#define SPHERE_H

#include "aabb.h"

#include <QString>

class Sphere
{
public:
    Sphere();
    Sphere(float x, float y, float z, float radius);
    void display();
    bool hasCollision(Sphere *s);
    bool hasCollision(AABB *p);
    float x, y, z, radius = 0;
};

#endif // SPHERE_H
