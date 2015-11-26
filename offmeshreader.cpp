#include "offmeshreader.h"
#include <QFile>
#include <QtCore>
#include <QTextStream>

OffMeshReader::OffMeshReader()
{
}

Mesh* OffMeshReader::load(QString& filepath) {

    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        qDebug() << "OffMeshReader::Could not open " << filepath;
        return nullptr;
    }

    Mesh* mesh = nullptr;
    QTextStream stream(&file);

    QString header;
    stream >> header;
    //Test if file is OFF file
    if(header.compare("OFF") != 0) {
        //NOT GOOD
        if(header.compare("nOFF") == 0) {
            qDebug() << "OffMeshReader::Unhandled dimension";
        }
        else {
            if(header.endsWith("OFF")) {
                qDebug() << "OffMeshReader::Unhandled off option";
            }
            else {
                qDebug() << "OffMeshReader::Not found OFF header";
            }
        }
        return nullptr;
    }


    //Mesh construction variables
    int pointsCount, facesCount, edgesCount = 0;
    QVector<QVector3D> points;// Points vector
    QVector<QVector<int>> indices; // Indices vector

    //Get header
    stream >> pointsCount >> facesCount >> edgesCount;

    //Get points
    for(int i = 0; i < pointsCount; ++i) {
        QVector3D xyz;
        double value;
        //Extract and build coordinates
        stream >> value;
        xyz.setX((float)value);
        stream >> value;
        xyz.setY((float)value);
        stream >> value;
        xyz.setZ((float)value);
        //Add point
        points.push_back(xyz);
    }

    //Get faces
    for(int i = 0; i< facesCount; ++i) {
        QVector<int> face;
        int id;
        //Extract indices
        stream >>id;
        face.push_back(id);
        stream >> id;
        face.push_back(id);
        stream >> id;
        face.push_back(id);
        //Add face
        indices.push_back(face);
    }
    mesh = new Mesh(points,indices);
    file.close();
    return mesh;
}
