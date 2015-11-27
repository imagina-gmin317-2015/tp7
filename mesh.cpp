#include "mesh.h"
#include <QVector3D>

Mesh::Mesh() {
}

Mesh::Mesh(QVector<QVector3D> &_points, QVector<QVector<int> > _indices):
    points(_points), indice_faces(_indices)
{

}


Mesh::~Mesh() {

}

QVector<QVector3D>& Mesh::getPoints() {
    return points;
}

QVector<QVector<int>>& Mesh::getIndices() {
        return indice_faces;
}

float* Mesh::generatePointsVector()
{
    float* v = new float[3*this->points.size()];
    int index=0;
    for(int i=0;i<points.size();++i)
    {
        v[index++]=points.at(i).x();
        v[index++]=points.at(i).y();
        v[index++]=points.at(i).z();
    }
    return v;
}
unsigned int* Mesh::generateIndicesVector()
{
    unsigned int* v = new unsigned int[3*this->indice_faces.size()];
    int index=0;
    for(int i=0;i<indice_faces.size();++i)
    {
        v[index++]=indice_faces.at(i).at(0);
        v[index++]=indice_faces.at(i).at(1);
        v[index++]=indice_faces.at(i).at(2);
    }
    return v;
}

float* Mesh::generateNormalsVector()
{
    float* f = new float[3*this->indice_faces.size()];
    int index=0;
    for(int i=0;i<indice_faces.size();++i)
    {
        QVector3D norm;
        //TODO
        //norm = QVector3D.normal(points.at(indice_faces.at(i).at(0)),
        //            points.at(indice_faces.at(i).at(1)),
        //            points.at(indice_faces.at(i).at(2)));
        f[index++] = norm.x();
        f[index++] = norm.y();
        f[index++] = norm.z();
    }
    return f;
}
