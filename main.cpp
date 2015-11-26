#include "trianglewindow.h"
#include "filemanager.h"
#include "plyloader.h"

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

#include <QtCore>
#include <QtGui>

#include <QDebug>

#include "sphere.h"
#include "aabb.h"

//#include <omp.h>

using namespace std;


int main(int argc, char **argv)
{
    srand(time(NULL));
    QGuiApplication app(argc, argv);
    
    QSurfaceFormat format;
    format.setSamples(16);
    
    paramCamera* c=new paramCamera();
    
    QTimer* calendar = new QTimer;
    FileManager *manager = FileManager::Instance();

    TriangleWindow* window[1];
    for(int i = 0; i < 1; i++)
    {
        if (i == 0)
            window[i] = new TriangleWindow(60);
        else
            window[i] = new TriangleWindow(30);
        window[i]->setSeason(i);
        window[i]->c = c;
        window[i]->setFormat(format);
        window[i]->resize(800,600);
        int x = i%2;
        int y = i>>1;
                
        window[i]->setPosition(x*500,y*450);
        window[i]->show();

        calendar->connect(calendar, SIGNAL(timeout()),window[i], SLOT(updateSeason()));

        manager->addWindow(window[i]);
    }
    
    calendar->start(20);

    Sphere *s1 = new Sphere(0.0f, 0.0f, 0.0f, 1.0f);
    Sphere *s2 = new Sphere(1.0f, 0.0f, 0.0f, 1.5f);
    Sphere *s3 = new Sphere(7.0f, 0.0f, 0.0f, 1.0f);

    AABB *a1 = new AABB(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
    AABB *a2 = new AABB(7.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    qDebug()<< "Sphere vs Sphere";

    qDebug()<< "Collision entre";
    s1->display();
    s2->display();
    qDebug()<< s1->hasCollision(s2);

    qDebug()<< "Collision entre";
    s1->display();
    s3->display();
    qDebug()<< s1->hasCollision(s3);

    qDebug()<< "Sphere vs ABB";

    qDebug()<< "Collision entre";
    s1->display();
    a1->display();
    qDebug()<< s1->hasCollision(a1);

    qDebug()<< "Collision entre";
    s1->display();
    a2->display();
    qDebug()<< s1->hasCollision(a2);

    return app.exec();
}

