#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <QMap>
#include "mesh.h"
#include "meshreader.h"
#include <QString>
#include <string>

class AssetManager
{
private:
    QMap<QString,Mesh*> meshMap;

    AssetManager();
public:
    static AssetManager& getInstance();

    ~AssetManager();

    /// Load a mesh from file with a MeshReader and store it with a name
    bool loadMesh(QString&, MeshReader*, QString&);
    /// Test if a named is used in MeshMap as Key
    bool haveNamedMesh(QString);
    /// Return the mesh named with the pass key
    Mesh* getNamedMesh(QString);

};

#endif // ASSETMANAGER_H
