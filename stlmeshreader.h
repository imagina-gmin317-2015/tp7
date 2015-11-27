#ifndef STLMESHREADER_H
#define STLMESHREADER_H

#include "mesh.h"
#include "meshreader.h"

class StlMeshReader : public MeshReader
{
public:
    StlMeshReader();

    virtual Mesh* load(QString &filepath);
};

#endif // STLMESHREADER_H
