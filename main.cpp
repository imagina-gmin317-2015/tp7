#include "gamewindow.h"

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
using namespace std;





int main(int argc, char **argv)
{
    srand(time(NULL));
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16);



    //1FPS
    /*GameWindow window1(60);
    window1.setFormat(format);
    window1.resize(960, 720);
    window1.setPosition(0,0);
    window1.show();


    window1.setAnimating(true);*/

    //30FPS
    GameWindow window30(60);
    window30.setFormat(format);
    window30.resize(960, 720);
    window30.setPosition(380,0);
    window30.show();

    window30.setAnimating(true);
/*
    //60FPS
    GameWindow window60(60);
    window60.setFormat(format);
    window60.resize(320, 240);
    window60.setPosition(0,290);
    window60.show();

    window60.setAnimating(true);

    //120FPS
    GameWindow window120(120);
    window120.setFormat(format);
    window120.resize(340, 240);
    window120.setPosition(380,290);
    window120.show();

    window120.setAnimating(true);
*/
    Camera *c = new Camera();
    Camera *c1 = new Camera();
    c->setEtat(6);

    //window1.setCam(c1);
    window30.setCam(c);
    //window60.setCam(c);
   // window120.setCam(c);



    return app.exec();
}



