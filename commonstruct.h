#ifndef COMMONSTRUCT
#define COMMONSTRUCT

#include <QVector3D>
#include <QVector2D>

struct color
{
    float r,g,b;
};

struct point
{
    float x, y ,z;
};


struct VertexData
{
    QVector3D position;
    QVector3D color;
    QVector2D texCoord;
    QVector3D normal;
};

enum Saison { PRINTEMPS = 0, ETE = 1, AUTOMNE = 2, HIVER = 3, NONE = 4};

#endif // COMMONSTRUCT

