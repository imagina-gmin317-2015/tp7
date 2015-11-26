#ifndef PLYLOADER_H
#define PLYLOADER_H

#include <QtCore>
#include <QtGui>
#include <QtGui/QOpenGLFunctions>

#include <QVector>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QRectF>

#include "aabb.h"

struct point{
    float x,y,z;
};

class PlyLoader
{
public:
    PlyLoader(QString fileName);
    void load();
    void draw();
    AABB* getBounding();
    QVector<float> getCoord();
    QString getName();

private:
    float x, y, z, ori, size;
    float xMin,xMax, yMin,yMax,zMin, zMax; // bounding box;
    QFile *file;
    QVector<point> vertices;
    QVector<QVector<int>> faces;

signals:

public slots:
};

#endif // PLYLOADER_H
