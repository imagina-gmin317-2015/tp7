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


GameWindow::GameWindow()
{
}

GameWindow::GameWindow(int i){
    fps = i;
    timer = new QTimer(this);
    timer->connect(timer, SIGNAL(timeout()), this, SLOT(renderNow()));
    timer->start(1000 / this->getFps());
}

int GameWindow::getFps(){
    return fps;
}

void GameWindow::setCam(Camera* c){
    cam = c;
}


void GameWindow::initialize()
{
    const qreal retinaScale = devicePixelRatio();


    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);


    loadMap(":/heightmap-2.png");
    loadMapQuadTree(":/heightmap-2.png",30.0);
    if(&cam == NULL){
        cam = new Camera();
    }

}

void GameWindow::loadMap(QString localPath)
{

    if (QFile::exists(localPath)) {
        m_image = QImage(localPath);
    }


    uint id = 0;
    p = new point[m_image.width() * m_image.height()];
    QRgb pixel;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {

            pixel = m_image.pixel(i,j);

            id = i*m_image.width() +j;

            p[id].x = (float)i/(m_image.width()) - ((float)m_image.width()/2.0)/m_image.width();
            p[id].y = (float)j/(m_image.height()) - ((float)m_image.height()/2.0)/m_image.height();
            p[id].z = 0.001f * (float)(qRed(pixel));
        }
    }
}

void GameWindow::loadMapQuadTree(QString localPath, float tolerance)
{

    if (QFile::exists(localPath)) {
        m_image = QImage(localPath);
    }

    relationsQT = (int**)malloc(sizeof(int*) * 250000);
    for(int  i = 0; i < 250000 ; i++){
        relationsQT[i] = (int*)malloc(sizeof(int) * 8);
    }
    cptQT = (int*)malloc(sizeof(int) * 250000);
    for(int  i = 0; i < 250000 ; i++){
        cptQT[i] = 0;
    }

    quadTree(m_image,tolerance, 0, m_image.width()-1,0, m_image.height()-1);
    qDebug()<<"Sur :"<<m_image.width() * m_image.height()<<endl;

}


void GameWindow::quadTree(QImage m_image, float tolerance, int widthMin, int widthMax, int heightMin, int heightMax){

    int i = widthMin;
    int j = heightMin;
    int id1 = i*m_image.width() +j;

    i = widthMax;
    j = heightMin;
    int id2 = i*m_image.width() +j;

    i = widthMin;
    j = heightMax;
    int id3 = i*m_image.width() +j;

    i = widthMax;
    j = heightMax;
    int id4 = i*m_image.width() +j;

    if(widthMin < widthMax -1 && heightMin < heightMax -1){
        float max1 = max(p[id1].z, p[id2].z);
        float max2 = max(max1, p[id3].z);
        float maxF = max(max2, p[id4].z);

        float min1 = min(p[id1].z, p[id2].z);
        float min2 = min(min1, p[id3].z);
        float minF = min(min2, p[id4].z);

        if(maxF-minF < 0.001f*tolerance){
            relationsQT[id1][cptQT[id1]++] = id2;
            relationsQT[id1][cptQT[id1]++] = id3;
            relationsQT[id1][cptQT[id1]++] = id4;

            relationsQT[id4][cptQT[id4]++] = id2;
            relationsQT[id4][cptQT[id4]++] = id3;
        }else{
            quadTree(m_image, tolerance, widthMin, (widthMax+widthMin)/2, heightMin, (heightMax+heightMin)/2);
            quadTree(m_image, tolerance, (widthMax+widthMin)/2, widthMax, heightMin, (heightMax+heightMin)/2);
            quadTree(m_image, tolerance, widthMin, (widthMax+widthMin)/2, (heightMax+heightMin)/2, heightMax);
            quadTree(m_image, tolerance, (widthMax+widthMin)/2, widthMax, (heightMax+heightMin)/2, heightMax);
        }
    }else{
        relationsQT[id1][cptQT[id1]++] = id2;
        relationsQT[id1][cptQT[id1]++] = id3;
        relationsQT[id1][cptQT[id1]++] = id4;

        relationsQT[id4][cptQT[id4]++] = id2;
        relationsQT[id4][cptQT[id4]++] = id3;
    }
}


void GameWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT);


    glLoadIdentity();

    if(tourniquet){
        cam->setRotY(cam->getRotY() + rotation);
    }


    cam->render();
/*
    glScalef(ss,ss,ss);
    glRotatef(rotX,1.0f,0.0f,0.0f);
    glRotatef(rotY,0.0f,0.0f,1.0f);
*/
    switch(cam->getEtat())
    {
    case 0:
        displayPoints();
        break;
    case 1:
        displayLines();
        break;
    case 2:
        displayTriangles();
        break;
    case 3:
        displayTrianglesC();
        break;
    case 4:
        displayTrianglesTexture();
        break;
    case 5:

        displayTrianglesTexture();
        displayLines();
        break;
    case 6:
        displayPointsQuadTree();
        break;
    case 7:
        displayLinesQuadTree();
        break;

    default:
        displayPoints();
        break;
    }
    ++m_frame;
}

bool GameWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:

        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case 'Z':
        cam->setSs(cam->getSs() + zoom);
        break;
    case 'S':
        cam->setSs(cam->getSs() - zoom);
        break;
    case 'A':
        cam->setRotX(cam->getRotX() + rotation);
        break;
    case 'E':
        cam->setRotX(cam->getRotX() - rotation);
        break;
    case 'Q':
        cam->setRotY(cam->getRotY() + rotation);
        break;
    case 'D':
        cam->setRotY(cam->getRotY() - rotation);
        break;
    case 'W':
        cam->setEtat(cam->getEtat() + 1);
        if(cam->getEtat() > 7)
            cam->setEtat(0);
        break;
    case 'C':
        tourniquet = !tourniquet;
        break;
    case 'P':
        fps *= 2;
        if(fps < 240){
            fps = 240;//limitation
        }
        timer->setInterval(1000 / fps);
        break;
    case 'M':
        fps /= 2;
        if(fps < 1){
            fps = 1;//limitation
        }
        timer->setInterval(1000 / fps);
        break;
    case 'X':
        carte ++;
        if(carte > 3)
            carte = 1;
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        loadMap(depth);
        break;

    }
    //renderNow();
}


void GameWindow::displayPoints()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    uint id = 0;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

        }
    }
    glEnd();
}

void GameWindow::displayPointsQuadTree()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for(int i = 0 ; i < m_image.width() * m_image.height() ;i++){
        for(int j = 0; j < cptQT[i]; j++){
            glVertex3f(
                        p[i].x,
                        p[i].y,
                        p[i].z
                        );
            glVertex3f(
                        p[relationsQT[i][j]].x,
                        p[relationsQT[i][j]].y,
                        p[relationsQT[i][j]].z
                        );
        }    }

    /*for(int i = 0; i < pQT->size(); i++)
    {
        glVertex3f(
            pQT->at(i).x,
            pQT->at(i).y,
            pQT->at(i).z);

    }*/
    glEnd();
}


void GameWindow::displayTriangles()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}

void GameWindow::displayTrianglesC()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);


            glColor3f(1.0f, 1.0f, 1.0f);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}


void GameWindow::displayLines()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = i*m_image.width() +(j+1);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +j+1;
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            id = (i+1)*m_image.width() +(j);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }

    glEnd();
}


void GameWindow::displayLinesQuadTree()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);

    for(int i = 0; i < m_image.width()*m_image.height();i++){
        for(int j = 0; j < cptQT[i]; j++){
            glVertex3f(
                        p[i].x,
                        p[i].y,
                        p[i].z
                        );
            glVertex3f(
                        p[relationsQT[i][j]].x,
                        p[relationsQT[i][j]].y,
                        p[relationsQT[i][j]].z
                        );
        }
    }


    /*
    for(int i = 0; i < pQT->size(); i+=4)
    {
            glVertex3f(
                        pQT->at(i).x,
                        pQT->at(i).y,
                        pQT->at(i).z);
            glVertex3f(
                        pQT->at(i+1).x,
                        pQT->at(i+1).y,
                        pQT->at(i+1).z);


            glVertex3f(
                        pQT->at(i).x,
                        pQT->at(i).y,
                        pQT->at(i).z);
            glVertex3f(
                        pQT->at(i+2).x,
                        pQT->at(i+2).y,
                        pQT->at(i+2).z);

            glVertex3f(
                        pQT->at(i).x,
                        pQT->at(i).y,
                        pQT->at(i).z);
            glVertex3f(
                        pQT->at(i+3).x,
                        pQT->at(i+3).y,
                        pQT->at(i+3).z);

            glVertex3f(
                        pQT->at(i+3).x,
                        pQT->at(i+3).y,
                        pQT->at(i+3).z);
            glVertex3f(
                        pQT->at(i+1).x,
                        pQT->at(i+1).y,
                        pQT->at(i+1).z);

            glVertex3f(
                        pQT->at(i+2).x,
                        pQT->at(i+2).y,
                        pQT->at(i+2).z);
            glVertex3f(
                        pQT->at(i+3).x,
                        pQT->at(i+3).y,
                        pQT->at(i+3).z);

    }*/

    glEnd();
}



void GameWindow::displayTrianglesTexture()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {

            id = i*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);



            id = i*m_image.width() +(j+1);
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;
            displayColor(p[id].z);
            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
        }
    }
    glEnd();
}


void GameWindow::displayColor(float alt)
{
    if (alt > 0.2)
    {
        glColor3f(01.0f, 1.0f, 1.0f);
    }
    else     if (alt > 0.1)
    {
        glColor3f(alt, 1.0f, 1.0f);
    }
    else     if (alt > 0.05f)
    {
        glColor3f(01.0f, alt, alt);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 1.0f);
    }

}
