#include "sphere.h"

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

bool Sphere::hasCollision(Sphere *s)
{
    return (sqrt(pow(this->x - s->x, 2) + pow(this->y - s->y, 2) + pow(this->z - s->z, 2)) <= this->radius + s->radius);
}

bool Sphere::hasCollision(AABB *p)
{

}
