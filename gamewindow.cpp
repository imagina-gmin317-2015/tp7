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
#include <QTimer>

#include <filemanager.h>


using namespace std;

#define PRINTEMPS 0
#define ETE 1
#define AUTOMNE 2
#define HIVER 3

#define MAX_PARTICLES 1000


#define HERBE 0
#define SNOW 1
#define MOUNTAIN 2
#define WATER 3
#define ORANGE_LEAVES 4

#define HERBE_NM 5
#define SNOW_NM 6
#define MOUNTAIN_NM 7
#define WATER_NM 8
#define ORANGE_NM 9

typedef struct
{
    float x;
    float y;
    float z;

}tParticle;

tParticle Particles[MAX_PARTICLES];


GameWindow::GameWindow(int fps, Camera *camera, int season)
{
    m_camera = camera;
    m_isRotating = false;
    m_season = season;

    m_objectLoaded = 0;

    initDay();

    m_neigeAlt = 0.04f;
    m_eauAlt = 0.03f;
    m_fps = fps;

    m_timer = new QTimer(this);
    m_timer->connect(m_timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    m_timer->start(1000/fps);

    m_timer_season = new QTimer(this);
    m_timer_season->connect(m_timer_season, SIGNAL(timeout()), this, SLOT(incrDay()));
    m_timer_season->start(20);
}

GameWindow::GameWindow(int fps, Camera *camera)
{
    m_camera = camera;
    m_isRotating = false;
    m_season = 0;
    m_neigeAlt = 0.04f;
    m_eauAlt = 0.03f;
    m_fps = fps;

    m_timer = new QTimer(this);
    m_timer->connect(m_timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    m_timer->start(1000/fps);
}

void GameWindow::setSeason(int m_season)
{
    this->m_season = m_season;
}

void GameWindow::initDay()
{
    if(m_season == HIVER)
        m_day = 80;
    else if(m_season == PRINTEMPS)
        m_day = 170;
    else if(m_season == ETE)
        m_day = 260;
    else if(m_season == AUTOMNE)
        m_day = 350;
}

int GameWindow::getDay() const
{
    return m_day;
}

int GameWindow::getSeason() const
{
    return m_season;
}

int GameWindow::getCarte() const
{
    return carte;
}

void GameWindow::incrDay()
{
    m_day = (m_day + 1) % 365;

    if(m_day == 80)
    {
        setSeason(HIVER);
    }
    else if(m_day == 170)
    {
        setSeason(PRINTEMPS);
    }
    else if(m_day == 260)
    {
        setSeason(ETE);
    }
    else if(m_day == 350)
    {
        setSeason(AUTOMNE);
    }

}


void GameWindow::setCamera(Camera *camera)
{
   this-> m_camera = camera;
}

Camera* GameWindow::getCamera()
{
    return this->m_camera;
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

    initall();

//    m_diffus = new QOpenGLShaderProgram(this);
//    m_diffus->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp7\\diffus_vertex_shader.vsh");
//    m_diffus->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp7\\diffus_fragment_shader.fsh");
    m_NM = new QOpenGLShaderProgram(this);
    m_NM->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\tp7\\NM_vertex_shader.vsh");
    m_NM->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\tp7\\NM_fragment_shader.fsh");


    if ( !m_NM->link() )
    {
        qWarning() << "Could not link";
        qDebug() << m_NM->log();
        return;
    }

    m_matrixUniform = m_NM->uniformLocation("matrix");

    glEnable(GL_DEPTH_TEST);
    glScalef(0.3f, 0.3f, 0.3f);

    glEnable(GL_LIGHTING);	// Active l'éclairage
    glEnable(GL_LIGHT0);	// Allume la lumière n°1

    glEnable( GL_COLOR_MATERIAL );

    float ambient[] = {0.8 , 0.8 , 0.8 , 1.0};
    float diffuse[] = {0.8 , 0.8 , 0.8 , 1.0};
    float position[] = {3.0 , 14.0 , 20.0 , 1.0};

    glLightfv(GL_LIGHT0 , GL_AMBIENT , ambient);
    glLightfv(GL_LIGHT0 , GL_DIFFUSE , diffuse);
    glLightfv(GL_LIGHT0 , GL_POSITION , position);

    loadTexture("..\\tp7\\herbe.jpg", tabTexture[HERBE]);
    loadTexture("..\\tp7\\snow.jpg", tabTexture[SNOW]);
    loadTexture("..\\tp7\\mountain.jpg", tabTexture[MOUNTAIN]);
    loadTexture("..\\tp7\\water.jpg", tabTexture[WATER]);
    loadTexture("..\\tp7\\orange_leaves.jpg", tabTexture[ORANGE_LEAVES]);

    loadTexture("..\\tp7\\herbe_NM.png", tabTexture[HERBE_NM]);
    loadTexture("..\\tp7\\snow_NM.png", tabTexture[SNOW_NM]);
    loadTexture("..\\tp7\\mountain_NM.png", tabTexture[MOUNTAIN_NM]);
    loadTexture("..\\tp7\\water_NM.png", tabTexture[WATER_NM]);
    loadTexture("..\\tp7\\orange_NM.png", tabTexture[ORANGE_NM]);

    quadtree = new QUADTREE();
    m_tailleTriangle = 12;
    //division(quadtree, 0, nW, 0, nH, 20, 4);



}

QImage GameWindow::loadTexture(char *filename, GLuint &textureID){
    glEnable(GL_TEXTURE_2D); // Enable texturing

    glGenTextures(1, &textureID); // Obtain an id for the texture
    glBindTexture(GL_TEXTURE_2D, textureID); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    QImage im(filename);

    QImage tex = QGLWidget::convertToGLFormat(im);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    glDisable(GL_TEXTURE_2D);

    return tex;
}


void GameWindow::loadCustomMap()
{
    FileManager fileManager;
    fileManager.readFile(this, this->title()+".save");
}

void GameWindow::loadObjectFile()
{
    if(m_objectLoaded == 5)
        m_tailleObject = 0.01f;
    else
        m_tailleObject = 0.05f;

    FileManager resources;
    resources.loadObjectMap(this, m_nameObject+".ply");

    point p1, p2;
    p1.x = 2.0f;
    p1.y = 2.0f;
    p1.z = 0.6f;

    p2.x = -0.5f;
    p2.y = -0.5f;
    p2.z = 0.6f;

    m_posObject = new point[2];
    m_posObject[0] = p1;
    m_posObject[1] = p2;

}

int GameWindow::getObjectLoaded() const
{
    return m_objectLoaded;
}

void GameWindow::setObjectLoaded(int objectLoaded)
{
    m_objectLoaded = objectLoaded;
}

int GameWindow::getNbObject() const
{
    return m_nbObject;
}

void GameWindow::setNbObject(int nbObject)
{
    m_nbObject = nbObject;
}

QString GameWindow::getNameObject() const
{
    return m_nameObject;
}

void GameWindow::setNameObject(const QString &nameObject)
{
    m_nameObject = nameObject;
}

float GameWindow::getTailleObject() const
{
    return m_tailleObject;
}



void GameWindow::setTailleObject(float tailleObject)
{
    m_tailleObject = tailleObject;
}

void GameWindow::showObjectMap()
{
    if(m_objectLoaded == 1)
        glColor3f(24.0f/255.0f, 57.0f/255.0f, 30.0f/255.0f);
    else if(m_objectLoaded == 2)
        glColor3f(148.0f/255.0f, 129.0/255.0f, 43.0f/255.0f);
    else if(m_objectLoaded == 3)
        glColor3f(1.0f, 0.5f, 0.0f);
    else if(m_objectLoaded == 4)
        glColor3f(1.0f, 1.0f, 1.0f);
    else
        glColor3f(187.0f/255.0f, 174.0f/255.0f, 152.0f/255.0f);


    for(int n = 0; n < m_nbObject; ++n)
    {
        for(int i = 0; i < m_nbFaces; ++i)
        {
            if(m_allFaces[i].d == -1)
            {
                glBegin(GL_TRIANGLES);
                glVertex3f((m_allSommets[m_allFaces[i].a].x + m_posObject[n].x) * m_tailleObject, (m_allSommets[m_allFaces[i].a].y + m_posObject[n].y) * m_tailleObject, (m_allSommets[m_allFaces[i].a].z + m_posObject[n].z) * m_tailleObject);
                glVertex3f((m_allSommets[m_allFaces[i].b].x + m_posObject[n].x) * m_tailleObject, (m_allSommets[m_allFaces[i].b].y + m_posObject[n].y) * m_tailleObject, (m_allSommets[m_allFaces[i].b].z + m_posObject[n].z) * m_tailleObject);
                glVertex3f((m_allSommets[m_allFaces[i].c].x + m_posObject[n].x) * m_tailleObject, (m_allSommets[m_allFaces[i].c].y + m_posObject[n].y) * m_tailleObject, (m_allSommets[m_allFaces[i].c].z + m_posObject[n].z) * m_tailleObject);
                glEnd();
            }
            else
            {
                glBegin(GL_QUADS);
                glVertex3f((m_allSommets[m_allFaces[i].a].x + m_posObject[n].x) * m_tailleObject, (m_allSommets[m_allFaces[i].a].y + m_posObject[n].y) * m_tailleObject, (m_allSommets[m_allFaces[i].a].z + m_posObject[n].z) * m_tailleObject);
                glVertex3f((m_allSommets[m_allFaces[i].b].x + m_posObject[n].x) * m_tailleObject, (m_allSommets[m_allFaces[i].b].y + m_posObject[n].y) * m_tailleObject, (m_allSommets[m_allFaces[i].b].z + m_posObject[n].z) * m_tailleObject);
                glVertex3f((m_allSommets[m_allFaces[i].c].x + m_posObject[n].x) * m_tailleObject, (m_allSommets[m_allFaces[i].c].y + m_posObject[n].y) * m_tailleObject, (m_allSommets[m_allFaces[i].c].z + m_posObject[n].z) * m_tailleObject);
                glVertex3f((m_allSommets[m_allFaces[i].d].x + m_posObject[n].x) * m_tailleObject, (m_allSommets[m_allFaces[i].d].y + m_posObject[n].y) * m_tailleObject, (m_allSommets[m_allFaces[i].d].z + m_posObject[n].z) * m_tailleObject);
                glEnd();
            }

        }
    }
}

int GameWindow::getNbFaces() const
{
    return m_nbFaces;
}

void GameWindow::setNbFaces(int nbFaces)
{
    m_nbFaces = nbFaces;
}

int GameWindow::getNbSommets() const
{
    return m_nbSommets;
}

void GameWindow::setNbSommets(int nbSommets)
{
    m_nbSommets = nbSommets;
}

face *GameWindow::getAllFaces() const
{
    return m_allFaces;
}

void GameWindow::setAllFaces(face *allFaces)
{
    m_allFaces = new face[m_nbFaces];
    for(int i = 0; i < m_nbFaces; ++i)
    {
        m_allFaces[i] = allFaces[i];
    }
    //m_allFaces = allFaces;

}

point *GameWindow::getPosObject() const
{
    return m_posObject;
}

void GameWindow::setPosObject(point *posObject)
{
    m_posObject = new point[m_nbObject];
    for(int i = 0; i < m_nbObject; ++i)
    {
        m_posObject[i] = posObject[i];
    }
}

point *GameWindow::getAllSommets() const
{
    return m_allSommets;
}

void GameWindow::setAllSommets(point *allSommets)
{
    m_allSommets = new point[m_nbSommets];
    for(int i = 0; i < m_nbSommets; ++i)
    {
        m_allSommets[i] = allSommets[i];
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

    nW = m_image.width();
    nH = m_image.height();

}

void GameWindow::render()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    m_camera->scale();

    if(m_isRotating)
        m_camera->setRotY(m_camera->getRotY() + 1);

    glRotatef(m_camera->getRotX(),1.0f,0.0f,0.0f);
    glRotatef(m_camera->getRotY(),0.0f,0.0f,1.0f);

    if(m_season == HIVER || m_season == AUTOMNE)
    {
        if(m_season == AUTOMNE)
            glColor3f(0.0f, 0.0f, 1.0f);
        else
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            glPointSize(2);
        }

        glBegin(GL_POINTS);
        for(int i = 0; i <= MAX_PARTICLES; i++)
        {
            glVertex3f(Particles[i].x, Particles[i].y, Particles[i].z);
        }

        glEnd();

        update();

    }

    m_NM->bind();

    QMatrix4x4 matrix;
    matrix.scale(m_camera->getSS());
    matrix.rotate(m_camera->getRotX(), 1, 0, 0);
    matrix.rotate(m_camera->getRotY(), 0, 0, 1);

    m_NM->setUniformValue(m_matrixUniform, matrix);

    if(m_objectLoaded > 0)
        showObjectMap();

    switch(m_camera->getEtat())
    {
    case 0:
        loadQuadtree(quadtree, 0, nW, 0, nH, 1);
        displayPointsQuadtree(quadtree);
        //displayPoints();
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
    default:
        displayPoints();
        break;
    }

    m_NM->release();
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
        --m_tailleTriangle;
        m_camera->setSS(m_camera->getSS() + 0.10f);
        break;
    case 'S':
        ++m_tailleTriangle;
        m_camera->setSS(m_camera->getSS() - 0.10f);
        break;
    case 'A':
        m_camera->setRotX(m_camera->getRotX() + 1.0f);
        break;
    case 'E':
        m_camera->setRotX(m_camera->getRotX() - 1.0f);
        break;
    case 'Q':
        m_camera->setRotY(m_camera->getRotY() + 1.0f);
        break;
    case 'D':
        m_camera->setRotY(m_camera->getRotY() - 1.0f);
        break;
    case 'W':
        m_camera->setEtat(m_camera->getEtat() + 1);
        if(m_camera->getEtat() > 5)
            m_camera->setEtat(0);
        break;
    case 'X':
    {
        m_neigeAlt = 0.04f;
        m_eauAlt = 0.03f;
        carte ++;
        if(carte > 3)
            carte = 1;
        QString depth (":/heightmap-");
        depth += QString::number(carte) ;
        depth += ".png" ;

        loadMap(depth);
        break;
    }
    case 'C':
    {
        m_isRotating = !m_isRotating;
        break;
    }
    case 'P':
    {
        if(m_fps < 1000)
        {
            m_fps *= 2;
            m_timer->stop();
            m_timer->start(1000/m_fps);
        }
        break;
    }
    case 'M':
    {
        if(m_fps > 2)
        {
            m_fps /= 2;
            m_timer->stop();
            m_timer->start(1000/m_fps);
        }

        break;
    }
    case 'K':
    {
        saveMap();
        break;

    }
    case 'J':
    {
        loadCustomMap();
        break;
    }
    case '1':
    {
        if(m_objectLoaded != 1)
        {
            m_nameObject = "springtree";
            m_objectLoaded = 1;
            m_nbObject = 2;
            loadObjectFile();

        }
        else
            m_objectLoaded = 0;

        break;
    }
    case '2':
    {
        if(m_objectLoaded != 2)
        {
            m_nameObject = "summertree";
            m_objectLoaded = 2;
            m_nbObject = 2;
            loadObjectFile();

        }
        else
            m_objectLoaded = 0;

        break;
    }
    case '3':
    {
        if(m_objectLoaded != 3)
        {
            m_nameObject = "autumntree";
            m_objectLoaded = 3;
            m_nbObject = 2;
            loadObjectFile();
        }
        else
            m_objectLoaded = 0;

        break;
    }
    case '4':
    {
        if(m_objectLoaded != 4)
        {
            m_nameObject = "wintertree";
            m_objectLoaded = 4;
            m_nbObject = 2;
            loadObjectFile();
        }
        else
            m_objectLoaded = 0;

        break;
    }
    case '5':
    {
        if(m_objectLoaded != 5)
        {
            m_nameObject = "tower";
            m_objectLoaded = 5;
            m_nbObject = 1;
            loadObjectFile();
        }
        else
            m_objectLoaded = 0;

        break;
    }
    }
    //renderNow();
}


void GameWindow::displayPoints()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    uint id = 0;
    for(int i = 0; i < m_image.width(); i++)
    {
        for(int j = 0; j < m_image.height(); j++)
        {

            id = i*m_image.width() +j;

            displayNormaleTexture(id);

            glBegin(GL_POINTS);

            glTexCoord2f((float)i/m_image.width(), (float)j/m_image.height());

            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            glEnd();

        }
    }

}


void GameWindow::displayTriangles()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    uint id = 0;

    for(int i = 0; i < m_image.width()-1; i++)
    {
        for(int j = 0; j < m_image.height()-1; j++)
        {
            id = i*m_image.width() +j;

            //displayTexture(id);
            displayNormaleTexture(id);

            glBegin(GL_TRIANGLES);

            glTexCoord2f((float)i/m_image.width(), (float)j/m_image.height());

            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = i*m_image.width() +(j+1);

            glTexCoord2f((float)i/m_image.width(), (float)(j+1)/m_image.height());

            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;

            glTexCoord2f((float)(i+1)/m_image.width(), (float)j/m_image.height());

            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            glEnd();

            id = i*m_image.width() +(j+1);

            //displayTexture(id);
            displayNormaleTexture(id);

            glTexCoord2f((float)i/m_image.width(), (float)(j+1)/m_image.height());

            glBegin(GL_TRIANGLES);

            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j+1;

            glTexCoord2f((float)(i+1)/m_image.width(), (float)(j+1)/m_image.height());

            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            id = (i+1)*m_image.width() +j;

            glTexCoord2f((float)(i+1)/m_image.width(), (float)j/m_image.height());

            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);
            glEnd();
        }
    }
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

void GameWindow::displayTexture(uint id)
{
    float alt = p[id].z;

    if(m_season == PRINTEMPS) // PRINTEMPS
    {
        if (alt > 0.08)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);
        }
        else if (alt > 0.03f)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);
        }
        else if(alt > 0.02f)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[HERBE]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER]);
        }

    }
    else if(m_season == ETE) // ETE
    {
        if (alt > 0.08)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);
        }
        else if (alt > 0.03f)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[HERBE]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER]);
        }
    }
    else if(m_season == AUTOMNE) // AUTOMNE
    {
        if(alt < m_eauAlt)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER]);
        }
        else if (alt > 0.17f)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);
        }
        else if (alt > 0.08)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[ORANGE_LEAVES]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[HERBE]);
        }

    }
    else if(m_season == HIVER) // HIVER
    {
        if (alt > 0.1f)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);
        }
        else if(alt > m_neigeAlt)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[SNOW]);
        }
        else if (alt > 0.015)
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[SNOW]);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER]);
        }
    }
}

void GameWindow::displayNormaleTexture(uint id)
{
    float alt = p[id].z;

    if(m_season == PRINTEMPS) // PRINTEMPS
    {
        if (alt > 0.08)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN_NM]);
        }
        else if (alt > 0.03f)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN_NM]);
        }
        else if(alt > 0.02f)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[HERBE]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[HERBE_NM]);
        }
        else
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER_NM]);
        }

    }
    else if(m_season == ETE) // ETE
    {
        if (alt > 0.08)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN_NM]);
        }
        else if (alt > 0.03f)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[HERBE]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[HERBE_NM]);
        }
        else
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER_NM]);
        }
    }
    else if(m_season == AUTOMNE) // AUTOMNE
    {
        if(alt < m_eauAlt)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER_NM]);
        }
        else if (alt > 0.17f)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN_NM]);
        }
        else if (alt > 0.08)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[ORANGE_LEAVES]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[ORANGE_NM]);
        }
        else
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[HERBE]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[HERBE_NM]);
        }

    }
    else if(m_season == HIVER) // HIVER
    {
        if (alt > 0.1f)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[MOUNTAIN_NM]);
        }
        else if(alt > m_neigeAlt)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[SNOW]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[SNOW_NM]);
        }
        else if (alt > 0.015)
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[SNOW]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[SNOW_NM]);
        }
        else
        {
            glUniform1i(m_NM->uniformLocation("texture1"), 0);
            glUniform1i(m_NM->uniformLocation("texture2"), 1);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tabTexture[WATER_NM]);
        }
    }
}

void GameWindow::displayColor(float alt)
{
    if(m_season == PRINTEMPS) // PRINTEMPS
    {
        if (alt > 0.08)
        {
            glColor3f(0.65f, 0.40f, 0.15f);
        }
        else if (alt > 0.03f)
        {
            glColor3f(0.12f, 0.62f, 0.33f);
        }
        else if(alt > 0.02f)
        {
            glColor3f(194.0/255.0, 247.0/255.0, 50.0/255.0);
        }
        else
        {
            glColor3f(0.0f, 0.5f, 1.0f);
        }

    }
    else if(m_season == ETE) // ETE
    {
        if (alt > 0.08)
        {
            glColor3f(0.65f, 0.40f, 0.15f);
        }
        else if (alt > 0.03f)
        {
            glColor3f(0.12f, 0.62f, 0.33f);
        }
        else
        {
            glColor3f(0.0f, 0.5f, 1.0f);
        }
    }
    else if(m_season == AUTOMNE) // AUTOMNE
    {
        if(alt < m_eauAlt)
        {
            glColor3f(0.0f, 0.5f, 1.0f);
        }
        else if (alt > 0.17f)
        {
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        else if (alt > 0.08)
        {
            glColor3f(0.65f, 0.40f, 0.15f);
        }
        else
        {
            glColor3f(0.8f, 0.33f, 0.0f);
        }

    }
    else if(m_season == HIVER) // HIVER
    {
        if (alt > 0.1f)
        {
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        else if(alt > m_neigeAlt)
        {
            glColor3f(223.0/255.0, 242.0/255.0, 1.0f);
        }
        else if (alt > 0.015)
        {
            glColor3f(0.65f, 0.40f, 0.15f);
        }
        else
        {
            glColor3f(0.0f, 1.0f, 1.0f);
        }
    }

}

void GameWindow::initall()
{
    for(int i = 0; i <= MAX_PARTICLES; i++)
    {
        Particles[i].x = (float)((float)(rand())/(float)(RAND_MAX) - 0.5f);
        Particles[i].y = (float)((float)(rand())/(float)(RAND_MAX) - 0.5f);
        Particles[i].z = (float)((float)(rand())/(float)(RAND_MAX) * 0.3f + 0.1f);
    }
}
void GameWindow::initentity(int index)
{
    Particles[index].x = (float)((float)(rand())/(float)(RAND_MAX) - 0.5f);
    Particles[index].y = (float)((float)(rand())/(float)(RAND_MAX) - 0.5f);
    Particles[index].z = (float)((float)(rand())/(float)(RAND_MAX) * 0.3f + 0.1f);
}

void GameWindow::update()
{
    for(int i = 0; i <= MAX_PARTICLES; i++)
    {
        if(Particles[i].z <= 0.1f)
            initentity(i);

        if(m_season == HIVER)
        {
            Particles[i].z -= (float)((float)(rand())/(float)(RAND_MAX) * 0.002f);
            Particles[i].x += (float)((float)(rand())/(float)(RAND_MAX) * 0.002f - 0.001f);

            if(m_neigeAlt > 0.025)
                m_neigeAlt -= 0.000000002f;

        }
        else if(m_season == AUTOMNE)
        {
            Particles[i].z -= (float)((float)(rand())/(float)(RAND_MAX) * 0.01f);
            if(m_eauAlt < 0.04)
                m_eauAlt += 0.000000002f;
        }
    }
}

void GameWindow::saveMap()
{
    qDebug() << "SAUVEGARDE EN COURS...." << endl;

    FileManager file(this, this->title());

    qDebug() << "SAUVEGARDE TERMINEE" << endl;
}

void GameWindow::setCarte(int value)
{
    carte = value;
}

void GameWindow::setDay(int day)
{
    m_day = day;
}


void GameWindow::loadQuadtree(QUADTREE * q, int indice1, int indice2, int indice3, int indice4, int S)
{
    int taille;

    if(m_tailleTriangle < 1)
        taille = 1;
    else
        taille = m_tailleTriangle;

    if(indice2 - indice1 <= m_tailleTriangle)
    {
        q->hGauche = NULL;
        q->hDroite = NULL;
        q->bGauche = NULL;
        q->bDroite = NULL;


        if(indice2 == nW)
            indice2 = nW -1;
        if(indice4 == nH)
            indice4 = nH -1;

        q->tabPoints[0] = indice1; // BAS GAUCHE
        q->tabPoints[1] = indice3;

        q->tabPoints[2] = indice2; // BAS DROITE
        q->tabPoints[3] = indice3;

        q->tabPoints[4] = indice1; // HAUT GAUCHE
        q->tabPoints[5] = indice4;

        q->tabPoints[6] = indice2; // HAUT DROITE
        q->tabPoints[7] = indice4;





        return;
    }


    double dividende = ((indice2-indice1)/2) * ((indice4-indice3)/2);


    int milieuH = (indice4 - indice3) / 2 + indice3;
    int milieuW = (indice2 - indice1) / 2 + indice1;


    int prob1[256] = {0};
    int prob2[256] = {0};
    int prob3[256] = {0};
    int prob4[256] = {0};


    for (int i = indice1; i < indice2; i++)
    {
        for (int j = indice3; j < indice4; j++)
         {
            if( i < milieuW && j < milieuH)
                ++prob1[(int)(p[i*nW + j].z*1000)];
            if( i < milieuW && j > milieuH )
                ++prob2[(int)(p[i*nW + j].z*1000)];
            if( i > milieuW && j < milieuH )
                ++prob3[(int)(p[i*nW + j].z*1000)];
            if( i > milieuW && j > milieuH )
                ++prob4[(int)(p[i*nW + j].z*1000)];
         }
    }

    int valMoy1 = 0;
    int valMoy2 = 0;
    int valMoy3 = 0;
    int valMoy4 = 0;

    double var1 = 0;
    double var2 = 0;
    double var3 = 0;
    double var4 = 0;


    for (int i = indice1; i < indice2; i++)
    {
        for (int j = indice3; j < indice4; j++)
        {

             if( i < milieuW && j < milieuH ){
                 valMoy3 += (int)(p[i*nW + j].z*1000);
                 var3 += ((double)prob1[(int)(p[i*nW + j].z*1000)]/dividende) * ((int)(p[i*nW + j].z*1000)*(int)(p[i*nW + j].z*1000));
             }
             if( i < milieuW && j > milieuH ){
                 valMoy1 += (int)(p[i*nW + j].z*1000);
                 var1 += ((double)prob2[(int)(p[i*nW + j].z*1000)]/dividende) * ((int)(p[i*nW + j].z*1000)*(int)(p[i*nW + j].z*1000));
             }
             if( i > milieuW && j < milieuH ){
                 valMoy4 += (int)(p[i*nW + j].z*1000);
                 var4 += ((double)prob3[(int)(p[i*nW + j].z*1000)]/dividende) * ((int)(p[i*nW + j].z*1000)*(int)(p[i*nW + j].z*1000));
             }
             if( i > milieuW && j > milieuH ){
                 valMoy2 += (int)(p[i*nW + j].z*1000);
                 var2 += ((double)prob4[(int)(p[i*nW + j].z*1000)]/dividende) * ((int)(p[i*nW + j].z*1000)*(int)(p[i*nW + j].z*1000));
             }

        }
    }

     valMoy1 /= dividende;
     valMoy2 /= dividende;
     valMoy3 /= dividende;
     valMoy4 /= dividende;

     var1 = (var1 - (valMoy1*valMoy1) )/dividende;
     var2 = (var2 - (valMoy2*valMoy2) )/dividende;
     var3 = (var3 - (valMoy3*valMoy3) )/dividende;
     var4 = (var4 - (valMoy4*valMoy4) )/dividende;


    QUADTREE * fHGauche = new QUADTREE();
    QUADTREE * fHDroite = new QUADTREE();
    QUADTREE * fBGauche = new QUADTREE();
    QUADTREE * fBDroite = new QUADTREE();


    if(var1 > S)
    {
        loadQuadtree(fHGauche, indice1, milieuW, milieuH, indice4, S); // HAUT GAUCHE
    }
    else
    {
        fHGauche->hGauche = NULL;
        fHGauche->hDroite = NULL;
        fHGauche->bGauche = NULL;
        fHGauche->bDroite = NULL;

        if(indice4 == nH)
            indice4 = nH - 1;

        fHGauche->tabPoints[0] = indice1;
        fHGauche->tabPoints[1] = indice4;

        fHGauche->tabPoints[2] = milieuW;
        fHGauche->tabPoints[3] = indice4;

        fHGauche->tabPoints[4] = indice1;
        fHGauche->tabPoints[5] = milieuH;

        fHGauche->tabPoints[6] = milieuW;
        fHGauche->tabPoints[7] = milieuH;


    }
    if(var2 > S)
    {
        loadQuadtree(fHDroite, milieuW, indice2, milieuH, indice4, S); // HAUT DROITE
    }
    else
    {
        fHDroite->hGauche = NULL;
        fHDroite->hDroite = NULL;
        fHDroite->bGauche = NULL;
        fHDroite->bDroite = NULL;


        if(indice4 == nW)
            indice4 = nW - 1;
        if(indice2 == nW)
            indice2 = nW - 1;

        fHDroite->tabPoints[0] = milieuW;
        fHDroite->tabPoints[1] = indice4;

        fHDroite->tabPoints[2] = indice2;
        fHDroite->tabPoints[3] = indice4;

        fHDroite->tabPoints[4] = milieuW;
        fHDroite->tabPoints[5] = milieuH;

        fHDroite->tabPoints[6] = indice2;
        fHDroite->tabPoints[7] = milieuH;


    }
    if(var3 > S)
    {
        loadQuadtree(fBGauche, indice1, milieuW, indice3, milieuH, S); // BAS GAUCHE
    }
    else
    {
        fBGauche->hGauche = NULL;
        fBGauche->hDroite = NULL;
        fBGauche->bGauche = NULL;
        fBGauche->bDroite = NULL;

        fBGauche->tabPoints[0] = indice1;
        fBGauche->tabPoints[1] = milieuH;

        fBGauche->tabPoints[2] = milieuW;
        fBGauche->tabPoints[3] = milieuH;

        fBGauche->tabPoints[4] = indice1;
        fBGauche->tabPoints[5] = indice3;

        fBGauche->tabPoints[6] = milieuW;
        fBGauche->tabPoints[7] = indice3;

    }
    if(var4 > S)
    {
        loadQuadtree(fBDroite, milieuW, indice2, indice3, milieuH, S);
    }
    else
    {

        fBDroite->hGauche = NULL;
        fBDroite->hDroite = NULL;
        fBDroite->bGauche = NULL;
        fBDroite->bDroite = NULL;

        if(indice2 == nW)
            indice2 -= 1;

        fBDroite->tabPoints[0] = milieuW;
        fBDroite->tabPoints[1] = milieuH;

        fBDroite->tabPoints[2] = indice2;
        fBDroite->tabPoints[3] = milieuH;

        fBDroite->tabPoints[4] = milieuW;
        fBDroite->tabPoints[5] = indice3;

        fBDroite->tabPoints[6] = indice2;
        fBDroite->tabPoints[7] = indice3;

    }

    q->hGauche = fHGauche;
    q->hDroite = fHDroite;
    q->bGauche = fBGauche;
    q->bDroite = fBDroite;

}


void GameWindow::displayPointsQuadtree(QUADTREE * q)
{
    if(q->hGauche == NULL)
    {
        for(int cpt = 0; cpt < 8; cpt += 2)
        {
            point pt;
            int id = q->tabPoints[cpt]*nW + q->tabPoints[cpt+1];
            pt = p[id];

            displayNormaleTexture(id);

            glBegin(GL_POINTS);

            glTexCoord2f((float)q->tabPoints[cpt]/m_image.width(), (float)q->tabPoints[cpt+1]/m_image.height());

            glVertex3f(
                        p[id].x,
                        p[id].y,
                        p[id].z);

            glEnd();
        }
    }
    else
    {
        displayPointsQuadtree(q->hGauche);
        displayPointsQuadtree(q->hDroite);
        displayPointsQuadtree(q->bGauche);
        displayPointsQuadtree(q->bDroite);
    }
}


