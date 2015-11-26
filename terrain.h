#ifndef TERRAIN_H
#define TERRAIN_H
#include "gameobject.h"
#include <QImage>
#include <QVector3D>
#include <GL/gl.h>
class Terrain : public virtual GameObject
{
    float* points;
    int width;
    int height;

public:
    explicit Terrain(GameObject *parent = 0);
    ~Terrain();
    void loadHeightmap(QString localPath);
    virtual void serialize(QDataStream& stream);
    static Terrain* unserialize(QDataStream& stream);
    int getWidth() const{
        return width;
    }
    int getHeight() const{
        return height;
    }

    int size() const {
        return 3 * width * height;
    }

    int coord(int i, int j) const {
        return 3*i*width+3*j+0;
    }

    float x(int coord) {
        return points[coord];
    }
    float y(int coord) {
        return points[coord+1];
    }

    float z(int coord){
        return points[coord+2];
    }

    float x(int i, int j) const {
        return points[coord(i,j)+0];
    }

    float y(int i, int j) const {
        return points[coord(i,j)+1];
    }

    float z(int i, int j) const {
        return points[coord(i,j)+2];
    }

    float* normal(int i,int j,int i2, int j2, int i3, int j3) {
            QVector3D a(x(i,j),y(i,j),z(i,j));
            QVector3D b(x(i2,j2),y(i2,j2),z(i2,j2));
            QVector3D c(x(i3,j3),y(i3,j3),z(i3,j3));


            QVector3D ab = b - a;
            QVector3D bc = c - b;

            QVector3D n(QVector3D::normal(ab,bc));

            float* norm = new float[3];

            norm[0] = n.x();
            norm[1] = n.y();
            norm[2] = n.z();

            return norm;
        }



signals:

public slots:

};

#endif // TERRAIN_H
