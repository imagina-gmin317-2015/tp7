#ifndef MESHREADER_H
#define MESHREADER_H

#include "mesh.h"

/// Classe abstraite définissant un objet lisant les Mesh
class MeshReader
{
public:
    MeshReader();
    virtual ~MeshReader();

    virtual Mesh* load(QString& filepath) = 0;
};

#endif // MESHREADER_H
