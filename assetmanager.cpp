#include "assetmanager.h"

#include "mesh.h"
#include "meshreader.h"
#include <QMap>
#include <QtCore>
#include <string>

AssetManager::AssetManager()
{

}

AssetManager &AssetManager::getInstance()
{
    static AssetManager instance;
    return instance;
}

AssetManager::~AssetManager()
{
    foreach (QString name, meshMap.keys()) {
        qDebug() << "delete mesh -> " << name;
        Mesh* m = meshMap[name];
        meshMap.remove(name);
        delete m;
    }
}


bool AssetManager::loadMesh(QString& meshname,MeshReader* reader, QString& meshfilepath) {

    if(meshMap.contains(meshname))
        return false;

    Mesh* mesh = reader->load(meshfilepath);
    if(mesh == nullptr)
        return false;

    meshMap.insert(meshMap.end(),meshname,mesh);
    return true;
}

Mesh* AssetManager::getNamedMesh(QString name) {
    if(meshMap.contains(name))
        return meshMap[name];

    return nullptr;
}

bool AssetManager::haveNamedMesh(QString name){
    return meshMap.contains(name);
}
