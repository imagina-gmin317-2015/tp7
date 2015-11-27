#include "plymeshreader.h"
#include <QFile>
#include <QtCore>
#include <QTextStream>
#include <QVector>

PlyMeshReader::PlyMeshReader() : MeshReader()
{
}

PlyMeshReader::~PlyMeshReader()
{

}

Mesh* PlyMeshReader::load(QString& filepath) {
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        qDebug() << "PlyMeshReader::Could not open " << filepath;
        return nullptr;
    }

    Mesh* mesh = nullptr;
    QTextStream stream(&file);

    QString header;
    int pointsCount = 0, facesCount = 0;
    QVector<QVector3D> points;
    QVector<QVector<int>> faces;

    //Explore header
    header = stream.readLine();
    if(! (header.compare("ply") == 0 || header.compare("PLY") == 0)) {
        qDebug() << "PlyMeshReader::File doesn't start ply header";
        return nullptr;
    }
    //file is ply-formatted file
    header = stream.readLine();
    do {
        header = stream.readLine();
    }while(header.startsWith("comment") || header.startsWith("file"));

    if(header.startsWith("element")) {
        QStringList list = header.split(' ');
        pointsCount = list.at(2).toInt();
    }
    int vertexPropertyCounter = 0;
    do {
        header = stream.readLine();
        vertexPropertyCounter++;
    } while(header.startsWith("property"));

    if(header.startsWith("element")) {
        QStringList list = header.split(' ');
        if(list.at(1).startsWith("face")) {
            facesCount = list.at(2).toInt();
        }
    }
    do {
        header = stream.readLine();
    }while(!header.startsWith("end_header"));

    //END HEADER
    //--------------------

    //START TO READ POINTS
    for(int i = 0; i < pointsCount; ++i) {
        header = stream.readLine();
        //Extract line of properties for a face
        QStringList tokens = header.split(' ');
        QVector3D point;
        point.setX(tokens.at(0).toFloat());//X
        point.setY(tokens.at(1).toFloat());//Y
        point.setZ(tokens.at(2).toFloat());//Z
        // ... other properties
        points.push_back(point);
    }
    //END POINTS
    //-------------------

    for(int i = 0; i < facesCount; ++i) {
        header = stream.readLine();
        //Extract line of properties for a face
        QStringList tokens = header.split(' ');
        QVector<int> face;
        int n = tokens.at(0).toInt();
        for(int k = 0; k < n; ++k) // indices of face
            face.push_back(tokens.at(1 + k).toInt());
        //... others properties
        faces.push_back(face);
    }

    mesh = new Mesh(points,faces);
    file.close();
    return mesh;
}
