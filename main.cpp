#include "gamewindow.h"
#include "assetmanager.h"
#include "mesh.h"
#include "meshreader.h"
#include "plymeshreader.h"
#include "offmeshreader.h"
#include "stlmeshreader.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <time.h>
#include <sys/time.h>
#include <iostream>

#include "filemanager.h"
#include <QtCore>
#include <QtGui>


using namespace std;


void serializeUnserializeTest() {
    QString name("./gameobject.bin");
    if(QFile::exists(name)) {
        QFile file(name);qDebug()<<"found";
        if(!file.open(QIODevice::ReadOnly)) {
            return;
        }
        QDataStream stream(&file);
        GameObject* g = GameObject::unserialize(stream);
        qDebug() << g->objectName();
        qDebug() << g->localPosition;
        qDebug() << g->localRotation;
        qDebug() << g->localScale;
        file.close();
    }
    else {
        qDebug() << "not found";
        QFile file(name);
        if(!file.open(QIODevice::WriteOnly)) {
            return;
        }
        QDataStream stream(&file);
        GameObject g("root");
        g.localPosition.setX(1);
        g.localPosition.setY(2.20f);
        g.localRotation.setZ(90.0f);
        g.localScale.setX(1.2f);
        g.serialize(stream);

        file.close();
    }
}

int main(int argc, char **argv)
{
    //serializeUnserializeTest();
    srand(time(NULL));
    QGuiApplication app(argc, argv);
    
    QSurfaceFormat format;
    format.setSamples(16);
    
    paramCamera* c=new paramCamera();
    
    QTimer* calendar = new QTimer;

    //Add ressources
    PlyMeshReader plyReader;


    QString spring(":/springtree.ply");QString sp_name("SpringTree");
    QString summer(":/summertree.ply");QString su_name("SummerTree");
    QString autumn(":/autumntree.ply");QString au_name("AutumnTree");
    QString winter(":/wintertree.ply");QString wi_name("WinterTree");
    AssetManager::getInstance().loadMesh(sp_name,&plyReader,spring);
    AssetManager::getInstance().loadMesh(su_name,&plyReader,summer);
    AssetManager::getInstance().loadMesh(au_name,&plyReader,autumn);
    AssetManager::getInstance().loadMesh(wi_name,&plyReader,winter);

    QVector<GameWindow*> window;

    //if big file set content
    if(FileManager::getInstance().haveBigFile()) {
        qDebug() << "Saved GameWindow";
        window = FileManager::getInstance().load();
        qDebug() << "after load"<< window.size();
    }
    else {
        //Default four
        qDebug() << "Default GameWindow";
        for(int i = 0; i < 4; i++)
        {
            if (i == 0)
                window.append(new GameWindow());
            else
                window.append(new GameWindow(30));
            Terrain* terrain = new Terrain();
            terrain->loadHeightmap(":/heightmap-1.png");
            window.at(i)->setSeason(i);
            window.at(i)->setTerrain(terrain);
            window.at(i)->c = c;
        }

    }

    for(int i=0; i<window.size(); ++i) {
        qDebug() << "t-"<<i;
        FileManager::getInstance().link(window[i]);
        window[i]->setFormat(format);
        window[i]->resize(500,375);
        int x = i%2;
        int y = i>>1;

        window[i]->setPosition(x*500,y*450);
        window[i]->show();
        calendar->connect(calendar, SIGNAL(timeout()),window[i], SLOT(updateSeason()));
    }
    
    calendar->start(20);

    int appResult = app.exec();
    //AssetManager::getInstance().purge();
    return appResult;
}

