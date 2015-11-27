#ifndef SCENE_H
#define SCENE_H

#include "gameobject.h"
#include <QList>

class Scene
{
private:
    QList<GameObject*> gameObjects;
public:
    Scene();
    virtual void addGameObject(GameObject*);
    virtual ~Scene();
};

#endif // SCENE_H
