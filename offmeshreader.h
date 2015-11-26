#ifndef OFFMESHREADER_H
#define OFFMESHREADER_H

#include "mesh.h"
#include "meshreader.h"

class OffMeshReader : public MeshReader
{
public:

    OffMeshReader();

    virtual Mesh* load(QString& filepath);
};

#endif // OFFMESHREADER_H
