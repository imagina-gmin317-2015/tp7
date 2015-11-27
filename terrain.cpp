#include "terrain.h"
#include <QImage>
#include <QFile>
#include <QtCore>

Terrain::Terrain(GameObject *parent) :
    GameObject(parent)
{
    width = 0;
    height = 0;
}

Terrain::~Terrain() {
    if(points != nullptr)
        delete points;
}

void Terrain::loadHeightmap(QString localPath)
{
    QImage heightmap;
    if (QFile::exists(localPath)) {
        heightmap = QImage(localPath);

    }
    else
        return;
    if(width>0 || height>0) {
        delete points;
    }

    width = heightmap.width();
    height = heightmap.height();
    uint id = 0;
    points = new float[3 * width * height];
    QRgb pixel;
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {

            pixel = heightmap.pixel(i,j);

            id = 3*i* width +3*j;

            points[id++] = (float)i/((float)width) - ((float)width/2.0)/((float)width);
            points[id++] = (float)j/((float)height) - ((float)height/2.0)/((float)height);
            points[id] = 0.001f * (float)(qRed(pixel));

        }
    }
}

void Terrain::serialize(QDataStream& stream) {
    GameObject::serialize(stream);
    stream << width << height;
    for(int i = 0; i < size(); ++i)
        stream << points[i];
}

Terrain* Terrain::unserialize(QDataStream& stream) {

    GameObject* gameObject = GameObject::unserialize(stream);

    Terrain* terrain = new Terrain();
    //Setup gameObject data
    terrain->setObjectName(gameObject->objectName());
    terrain->localPosition = gameObject->localPosition;
    terrain->localRotation = gameObject->localRotation;
    terrain->localScale = gameObject->localScale;
    delete gameObject;


    int width, height;
    stream >> width;
    stream >> height;
    uint id = 0;
    terrain->points = new float[3 * width* height];
    terrain->width = width;
    terrain->height = height;
    float value;
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            id = 3*i*width +3*j;

            stream >> value;
            terrain->points[id++] = value;//X
            stream >> value;
            terrain->points[id++] = value;//Y
            stream >> value;
            terrain->points[id] = value;//Z
        }
    }
    return terrain;
}
