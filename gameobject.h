#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QVector3D>

class GameObject: public QObject
{
Q_OBJECT
public:
    QVector3D localPosition = QVector3D(0,0,0);
    QVector3D localRotation = QVector3D(0,0,0);
    QVector3D localScale = QVector3D(1,1,1);
    /**
     * @brief GameObject Simple Constructeur
     * @param _name
     */
    explicit GameObject(GameObject* parent = 0);
    explicit GameObject(QString name, GameObject* parent=0);
    /**
     * @brief ~GameObject
     */
    virtual ~GameObject();

    virtual GameObject* parent();

    virtual QObjectList children();

    virtual void serialize(QDataStream& stream);

    static GameObject* unserialize(QDataStream& stream);

};

#endif // GAMEOBJECT_H
