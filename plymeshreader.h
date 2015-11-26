#ifndef PLYMESHREADER_H
#define PLYMESHREADER_H
#include "mesh.h"
#include "meshreader.h"

class PlyMeshReader : public virtual MeshReader
{
public:
    PlyMeshReader();
    virtual ~PlyMeshReader();

    virtual Mesh* load(QString& filepath);
};

#endif // PLYMESHREADER_H
