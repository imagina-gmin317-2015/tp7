#ifndef DROUGHT_H
#define DROUGHT_H

#include <QtGui/QOpenGLFunctions>
#include <QDebug>

#include "resourcemanager.h"
#include "birds.h"
#include "octree.h"

class Drought
{
public:
    Drought();
    void update(float delta);
    void draw();
    float getYellow() const;
    float getSnowHeightModifier() const;
    void setActive(bool active);
    bool active() const;

    QVector<data *> attributes();

private:
    float yellow;
    float snowHeight;
    bool isActive;
    Birds *birds;
    Octree *octree;
};

#endif // DROUGHT_H
