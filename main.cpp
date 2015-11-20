#include "gamewindow.h"
#include "mytcpserver.h"

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

    MyTcpServer* tcpServer = new MyTcpServer();
    tcpServer->start(10000);

    GameWindow window0(40);
    window0.setFormat(format);
    window0.resize(500,375);
    window0.setPosition(0,0);
    window0.show();

    /*GameWindow window1(40);
    window1.setFormat(format);
    window1.resize(500,375);
    window1.setPosition(800,50);
    window1.show();

    GameWindow window2(40);
    window2.setFormat(format);
    window2.resize(500,375);
    window2.setPosition(50,400);
    window2.show();

    GameWindow window3(40);
    window3.setFormat(format);
    window3.resize(500,375);
    window3.setPosition(800,400);
    window3.show();*/

    window0.setAnimating(true);
    /*window1.setAnimating(true);
    window2.setAnimating(true);
    window3.setAnimating(true);*/

    return app.exec();
}



