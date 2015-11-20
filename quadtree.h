#ifndef QUADTREE_H
#define QUADTREE_H

#include <QVector3D>
#include <QtGui/QOpenGLFunctions>
#include <vector>

#include "Voxel.h"
#include "commonstruct.h"

class Quadtree
{
public:
    Quadtree(VertexData* vert, float delt, int resolution, int terrain_width);

    void QuadtreeStatic(Voxel* v, int offsetX, int offsetY);
    void Reset(VertexData* vert);
    void DisplayQuadtree();

    std::vector<Voxel*> getVoxels() const { return voxels; };
    int getNbVoxels() const { return voxels.size(); };

    void setDelta(float d) { delta = d; };
    void setResolution(int d) { resolution = d; };

private:
    Voxel* Subdivide(Voxel* v);
    float ComputeHeightAverage(int offsetX, int offsetY, int length);

private:
    float delta;
    int resolution;
    int terrain_width;

    VertexData* vertices;

    std::vector<Voxel*> voxels;

signals:

public slots:
};

#endif // QUADTREE_H
