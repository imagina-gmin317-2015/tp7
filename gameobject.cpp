#include "gameobject.h"
#include <QDataStream>
#include <QtCore>

GameObject::GameObject(GameObject* parent) : GameObject("GameObject",parent)
{

}

GameObject::GameObject(QString name, GameObject* parent) : QObject(parent)
{
    setObjectName(name);
}

GameObject::~GameObject() {
}


GameObject* GameObject::parent() {
    return dynamic_cast<GameObject*>(QObject::parent());
}


QObjectList GameObject::children(){
    QObjectList children = QObject::children();
    //TODO Replace by
    return children;
}

void GameObject::serialize(QDataStream& stream) {
    (stream) << objectName();qDebug() << objectName();
    (stream) << localPosition << localRotation << localScale;
}

GameObject* GameObject::unserialize(QDataStream& stream) {
    GameObject* g = new GameObject();
    QString name;
    (stream) >> name;
    g->setObjectName(name);
    (stream )>> g->localPosition >> g->localRotation >> g->localScale;
    return g;
}
