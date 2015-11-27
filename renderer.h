#ifndef RENDERER_H
#define RENDERER_H
#include "gameobject.h"
#include "mesh.h"
class Renderer : public virtual GameObject
{
public:
    Mesh* m_mesh;
    explicit Renderer(Mesh* mesh, GameObject* parent=0);
    virtual void draw();
};

#endif // RENDERER_H
