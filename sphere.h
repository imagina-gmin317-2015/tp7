#ifndef SPHERE_H
#define SPHERE_H

#include "aabb.h"

class Sphere
{
public:
    Sphere();
    Sphere(float x, float y, float z, float radius);
    bool hasCollision(Sphere *s);
    bool hasCollision(AABB *p);
    float x, y, z, radius = 0;
};

#endif // SPHERE_H
