#include "scene.h"

Scene::Scene()
{
}

Scene::~Scene() {

}


void Scene::addGameObject(GameObject *object) {
    gameObjects.append(object);
}

