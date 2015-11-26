#ifndef AABB_H
#define AABB_H

class AABB
{
public:
    AABB();
    AABB(float x, float y, float z, float w, float h, float d);
    bool hasCollision(AABB *b);
    bool isIn(float x, float y, float z);
    void display();
    float x, y, z, w, h, d = 0;
};

#endif // AABB_H
