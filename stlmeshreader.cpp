#include "stlmeshreader.h"
#include <QFile>
#include <QtCore>
#include <QTextStream>
#include <QVector>

StlMeshReader::StlMeshReader()
{

}

Mesh *StlMeshReader::load(QString &filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        qDebug() << "StlMeshReader::Could not open " << filepath;
        return nullptr;
    }

    Mesh* mesh = nullptr;
    QTextStream stream(&file);

    QString line;
    line = stream.readLine();
    if(!line.startsWith("solid")) {
        qDebug() << "StlMeshReader::Bad header";
        return nullptr;
    }

    int index = 0;
    QVector<QVector3D> points;
    QVector<QVector<int>> faces;
    while(!line.startsWith("endsolid") ) {

        line = stream.readLine();

        if(line.startsWith("facet")) {
            //Contains normal
            while(! line.startsWith("endfacet")) {

                line = stream.readLine();
                if(line.startsWith("outer loop")) {

                    //Start reading vertex of face
                    QVector<int> face;
                    while(! line.startsWith("endloop")) {

                        line = stream.readLine();
                        if(line.startsWith("vertex")) {
                            QStringList tokens = line.split(" ");
                            face.push_back(index++);
                            QVector3D point;
                            //Link xyz
                            point.setX(tokens.at(1).toFloat());//X
                            point.setY(tokens.at(2).toFloat());//X
                            point.setZ(tokens.at(3).toFloat());//X
                            points.push_back(point);//Add vertex
                        }

                    }
                    faces.push_back(face);//Add face
                }
            }
        }
    }

    mesh = new Mesh(points, faces);
    return mesh;
}

//Read solid
