#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QVector3D>

class Mesh
{
private:
    /// Liste de points du maillage
    QVector<QVector3D> points;

    /// Liste de triplets d'indices pour les faces du maillage
    QVector<QVector<int>> indice_faces;

public:
    Mesh();
    Mesh(QVector<QVector3D>& _points, QVector<QVector<int>> _indices);
    virtual ~Mesh();

    /// Return a vector of vextex
    virtual QVector<QVector3D>& getPoints();
    /// Return a vector of indices (all dimensions)
    virtual QVector<QVector<int>>& getIndices();

    /// Generation of a vector of points in float (x1,y1,z1,x2,y2,....)
    virtual float* generatePointsVector();
    /// Generation of a vector of indices for 3point-based faces in float (i1,j1,k1,i2,j2,k2,....)
    virtual unsigned int* generateIndicesVector();
    /// Generation of a vector of normals for faces (based on declared faces)
    virtual float* generateNormalsVector();

};

#endif // MESH_H
