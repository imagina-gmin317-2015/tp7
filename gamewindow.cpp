#include "gamewindow.h"
#include "mesh.h"
#include "meshreader.h"
#include "plymeshreader.h"
#include "assetmanager.h"
#include "filemanager.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include "quadtree.h"
#include <QtCore/qmath.h>
#include <QMouseEvent>
#include <QKeyEvent>

#include <QtCore>
#include <QtGui>

//#include <omp.h>

int numParticules = 1000;
int minP = 0;
int maxP = 360;


using namespace std;

GameWindow::GameWindow()
{
    nbTick = 0;
    m_frame = 0;
    maj = 20;
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
    timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    timer->start(maj);

    master = true;
}
GameWindow::GameWindow(int _maj)
{
    terrain = nullptr;
    nbTick = 0;
    m_frame = 0;
    maj = _maj;
    QString s ("FPS : ");
    s += QString::number(1000/maj);
    s += "(";
    s += QString::number(maj);
    s += ")";
    setTitle(s);
    timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),this, SLOT(renderNow()));
    timer->start(maj);
}

void GameWindow::setSeason(int i)
{
    season = i;

    if (i==0) day=80;
    else if (i==1) day = 170;
    else if (i==2) day = 260;
    else if (i==3) day = 350;
}

void GameWindow::setTerrain(Terrain *terrain) {
    this->terrain = terrain;
}

void GameWindow::updateSeason()
{
    day = (day + 1) % 365;

    if (day==80) season = 0;
    else if (day==170) season = 1;
    else if (day==260) season = 2;
    else if (day==350) season = 3;
}

void GameWindow::initialize()
{
    const qreal retinaScale = devicePixelRatio();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glViewport(0, 0, this->width() * retinaScale, this->height() * retinaScale);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);

    //particules sys
    particules = new point[numParticules];

    for(int i = 0; i < numParticules; i++)
    {
        int angle =minP + (rand() % (int)(maxP - minP + 1));
        int dist = (rand() % (int)(100 ));
        int alt = (rand() % (int)(100));
        float x = sin(
                      ((3.14159 * 2) *
                       angle
                       )/360
                      )*dist;
        float y = cos(
                      ((3.14159 * 2) *
                       angle
                       )/360
                      )*dist;

        particules[i].x = 0.5f + (float)+x/(terrain->getWidth()) - ((float)terrain->getWidth()/2.0)/terrain->getWidth();
        particules[i].y = 0.5f + (float)y/(terrain->getHeight()) - ((float)terrain->getHeight()/2.0)/terrain->getHeight();
        particules[i].z = (float)(alt)/100;
    }

    //trees sys
    for(int k= 0;k<4;++k) {
        Renderer* renderer = new Renderer(AssetManager::getInstance().getNamedMesh("WinterTree"),terrain);
        int i,j=0;
        i = (rand() %(int)(terrain->getWidth()));
        j = (rand() %(int)(terrain->getHeight()));
        renderer->localPosition.setX(terrain->x(i,j));
        renderer->localPosition.setY(terrain->y(i,j));
        renderer->localPosition.setZ(terrain->z(i,j));
        renderer->localScale.setX(.02f);
        renderer->localScale.setY(.02f);
        renderer->localScale.setZ(.02f);
        QString treeName("tree");
        treeName+="-";
        treeName+=k;
        renderer->setObjectName(treeName);
        renderers.append(renderer);
    }

    //quadset_indices = QuadTree_func(terrain,quadset_indices,.20f,&quadset_size,0,0,terrain->getWidth()-1,terrain->getHeight()-1);
    qt = new QuadTree(terrain);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    initializeLighting();
    qt->tolerate(tolerate,min_patch);
}

void GameWindow::initializeLighting() {
    qDebug() << "start lumens";
    glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);

     // Light model parameters:
     // -------------------------------------------

     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

     // -------------------------------------------
     // Spotlight Attenuation

     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;

     //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     //glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     //glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;

     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);


     // -------------------------------------------
     // Lighting parameters:

     GLfloat light_pos[] = {0.0f, 5.0f, 10.0f, 0.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.0f, .50f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {1.0f, 1.0f, 1.0f, 1.0f};
     GLfloat material_Ks[] = {0.1f, 1.f, 0.30f, 1.0f};
     GLfloat material_Ke[] = {0.0f, 0.40f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
}




void GameWindow::render()
{
    nbTick += maj;

    if(nbTick >= 1000)
    {
        QString s ("FPS : ");
        s += QString::number(m_frame);
        s += "(th=";
        s += QString::number(1000/maj);
        s += "-";
        s += QString::number(maj);
        s += ")";
        s += " day ";
        s += QString::number(day);
        if (season==0) s += " Printemps ";
        else if (season==1) s += " Eté ";
        if (season==2) s += " Automne ";
        if (season==3) s += " Hiver ";
        setTitle(s);
        nbTick = 0;
        m_frame = 0;
    }
    glClear(GL_COLOR_BUFFER_BIT);


    glLoadIdentity();
    glScalef(c->ss,c->ss,c->ss);

    glRotatef(c->rotX,1.0f,0.0f,0.0f);
    if(c->anim == 0.0f)
    {
        glRotatef(c->rotY,0.0f,0.0f,1.0f);
    }
    else
    {
        glRotatef(c->anim,0.0f,0.0f,1.0f);
        if(master)
            c->anim +=0.05f;
    }

    switch(c->etat)
    {
        case 0:
            displayPoints();
            break;
        case 1:
            glDisable(GL_LIGHTING);
            displayLines();
            glEnable(GL_LIGHTING);
            break;
        case 2:
            displayTriangles();
            break;
        case 3:
            glDisable(GL_LIGHTING);
            displayTrianglesC();
            glEnable(GL_LIGHTING);

            break;
        case 4:
            displayTrianglesTexture();
            break;
        case 5:

            displayTrianglesTexture();
            glDisable(GL_LIGHTING);
            displayLines();
            break;
        case 6:
            glEnable(GL_LIGHTING);
            displayQuadTree();
            glDisable(GL_LIGHTING);
        break;
        default:
            displayPoints();
            break;
    }
    if (season == 2)
        updateParticlesAut();
    else if (season == 3)
        updateParticlesHiv();

     //TODO Draw a Tree
    glColor3f(.2f,1.0f,.0f);
    foreach(Renderer* renderer, renderers) {
        if(season == 0)
            renderer->m_mesh = AssetManager::getInstance().getNamedMesh("SpringTree");
        else if(season == 1)
            renderer->m_mesh = AssetManager::getInstance().getNamedMesh("SummerTree");
        else if(season == 2)
            renderer->m_mesh = AssetManager::getInstance().getNamedMesh("AutumnTree");
        else
            renderer->m_mesh = AssetManager::getInstance().getNamedMesh("WinterTree");
        renderer->draw();
    }

    m_frame++;

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

        case 'C':
            if(c->anim == 0.0f)
                c->anim = c->rotY;
            else
                c->anim = 0.0f;
            break;
        case 'Z':
            c->ss += 0.10f;
            break;
        case 'S':
            c->ss -= 0.10f;
            break;
        case 'A':
            c->rotX += 1.0f;
            break;
        case 'E':
            c->rotX -= 1.0f;
            break;
        case 'Q':
            c->rotY += 1.0f;
            break;
        case 'D':
            c->rotY -= 1.0f;
            break;
        case 'W':
            c->etat ++;
            if(c->etat > 6)
                c->etat = 0;
            qDebug() << "current state: "<<c->etat;
            break;
        case 'T':
            FileManager::getInstance().save();
            qDebug() << "save";
        case '+':
            tolerate-=0.01f;
            qDebug() << "subdivide";
            qDebug() << "tolerance level : " << tolerate;
            qt->tolerate(tolerate,min_patch);
        break;
        case '-':
            tolerate+=.01f;
            qDebug() << "decimate";
            qt->tolerate(tolerate,min_patch);
        break;
        case 'P':
            maj++;
            timer->stop();
            timer->start(maj);
            break;
        case 'O':
            maj--;
            if(maj < 1)
                maj = 1;
            timer->stop();
            timer->start(maj);
            break;
        case 'L':
            maj = maj - 20;
            if(maj < 1)
                maj = 1;
            timer->stop();
            timer->start(maj);
            break;
        case 'M':
            maj = maj + 20;

            timer->stop();
            timer->start(maj);
            break;
        case 'X':
            carte ++;
            if(carte > 3)
                carte = 1;
            QString depth (":/heightmap-");
            depth += QString::number(carte) ;
            depth += ".png" ;

            terrain->loadHeightmap(depth);
            qt->clear();
            delete qt;
            qt = new QuadTree(terrain);
            qt->tolerate(tolerate,min_patch);
            break;


    }

}


void GameWindow::displayPoints()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    //uint id = 0;
    for(int i = 0; i < terrain->getWidth(); i++)
    {
        for(int j = 0; j < terrain->getHeight(); j++)
        {
            //id = i*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i,j),
                       terrain->y(i,j),
                       terrain->z(i,j));

        }
    }
    glEnd();
}

//void GameWindow::displayQuadTree() {

//}


void GameWindow::displayTriangles()
{
    if (season==0) glColor3f(0.5f, 0.8f, 0.5f);
    else if (season==1) glColor3f(0.8f,0.5f, 0.5f);
    else if (season==2) glColor3f(0.5f, 0.5f, 0.8f);
    else if (season==3) glColor3f(1.0f, 1.0f, 1.0f);


    glBegin(GL_TRIANGLES);
    //uint id = 0;

    for(int i = 0; i < terrain->getWidth()-1; i++)
    {
        for(int j = 0; j < terrain->getHeight()-1; j++)
        {

            glNormal3fv(terrain->normal(i,j,i,j+1,i+1,j));
            //id = i*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i,j),
                       terrain->y(i,j),
                       terrain->z(i,j));
            //id = i*terrain->getWidth() +(j+1);
            glVertex3f(
                       terrain->x(i,j+1),
                       terrain->y(i,j+1),
                       terrain->z(i,j+1));
            //id = (i+1)*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i+1,j),
                       terrain->y(i+1,j),
                       terrain->z(i+1,j));


            glNormal3fv(terrain->normal(i,j+1,i+1,j+1,i+1,j));
            //id = i*terrain->getWidth() +(j+1);
            glVertex3f(
                       terrain->x(i,j+1),
                       terrain->y(i,j+1),
                       terrain->z(i,j+1));
            //id = (i+1)*terrain->getWidth() +j+1;
            glVertex3f(
                       terrain->x(i+1,j+1),
                       terrain->y(i+1,j+1),
                       terrain->z(i+1,j+1));
            //id = (i+1)*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i+1,j),
                       terrain->y(i+1,j),
                       terrain->z(i+1,j));
        }
    }

    glEnd();
}

void GameWindow::displayTrianglesC()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    //uint id = 0;

    for(int i = 0; i < terrain->getWidth()-1; i++)
    {
        for(int j = 0; j < terrain->getHeight()-1; j++)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
            glNormal3fv(terrain->normal(i,j,i,j+1,i+1,j));
            glVertex3f(
                       terrain->x(i,j),
                       terrain->y(i,j),
                       terrain->z(i,j));
            //id = i*terrain->getWidth() +(j+1);
            glVertex3f(
                       terrain->x(i,j+1),
                       terrain->y(i,j+1),
                       terrain->z(i,j+1));
            //id = (i+1)*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i+1,j),
                       terrain->y(i+1,j),
                       terrain->z(i+1,j));


            glColor3f(1.0f, 1.0f, 1.0f);
            glNormal3fv(terrain->normal(i,j+1,i+1,j+1,i+1,j));

            //id = i*terrain->getWidth() +(j+1);
            glVertex3f(
                       terrain->x(i,j+1),
                       terrain->y(i,j+1),
                       terrain->z(i,j+1));
            //id = (i+1)*terrain->getWidth() +j+1;
            glVertex3f(
                       terrain->x(i+1,j+1),
                       terrain->y(i+1,j+1),
                       terrain->z(i+1,j+1));
            //id = (i+1)*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i+1,j),
                       terrain->y(i+1,j),
                       terrain->z(i+1,j));
        }
    }
    glEnd();
}


void GameWindow::displayLines()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    //uint id = 0;

    for(int i = 0; i < terrain->getWidth()-1; i++)
    {
        for(int j = 0; j < terrain->getHeight()-1; j++)
        {

            //id = i*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i,j),
                       terrain->y(i,j),
                       terrain->z(i,j));
            //id = i*terrain->getWidth() +(j+1);
            glVertex3f(
                       terrain->x(i,j+1),
                       terrain->y(i,j+1),
                       terrain->z(i,j+1));

            //id = (i+1)*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i+1,j),
                       terrain->y(i+1,j),
                       terrain->z(i+1,j));
            //id = i*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i,j),
                       terrain->y(i,j),
                       terrain->z(i,j));

            //id = (i+1)*terrain->getWidth() +j;
            glVertex3f(
                       terrain->x(i+1,j),
                       terrain->y(i+1,j),
                       terrain->z(i+1,j));
            //id = i*terrain->getWidth() +(j+1);
            glVertex3f(
                       terrain->x(i,j+1),
                       terrain->y(i,j+1),
                       terrain->z(i,j+1));

            //id = i*terrain->getWidth() +(j+1);
            glVertex3f(
                       terrain->x(i,j+1),
                       terrain->y(i,j+1),
                       terrain->z(i,j+1));
            //id = (i+1)*terrain->getWidth() +j+1;
            glVertex3f(
                       terrain->x(i+1,j+1),
                       terrain->y(i+1,j+1),
                       terrain->z(i+1,j+1));

            //id = (i+1)*terrain->getWidth() +j+1;
            glVertex3f(
                       terrain->x(i+1,j+1),
                       terrain->y(i+1,j+1),
                       terrain->z(i+1,j+1));

            //id = (i+1)*terrain->getWidth() +(j);
            glVertex3f(
                       terrain->x(i+1,j),
                       terrain->y(i+1,j),
                       terrain->z(i+1,j));
        }
    }

    glEnd();
}

void GameWindow::displayTrianglesTexture()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    //uint id = 0;

    for(int i = 0; i < terrain->getWidth()-1; i++)
    {
        for(int j = 0; j < terrain->getHeight()-1; j++)
        {

            //id = i*terrain->getWidth() +j;
            displayColor(terrain->z(i,j));
            glNormal3fv(terrain->normal(i,j,i,j+1,i+1,j));
            glVertex3f(
                       terrain->x(i,j),
                       terrain->y(i,j),
                       terrain->z(i,j));
            //id = i*terrain->getWidth() +(j+1);
            displayColor(terrain->z(i,j));
            glVertex3f(
                       terrain->x(i,j+1),
                       terrain->y(i,j+1),
                       terrain->z(i,j+1));
            //id = (i+1)*terrain->getWidth() +j;
            displayColor(terrain->z(i+1,j));
            glVertex3f(
                       terrain->x(i+1,j),
                       terrain->y(i+1,j),
                       terrain->z(i+1,j));


           // id = i*terrain->getWidth() +(j+1);
            glNormal3fv(terrain->normal(i,j+1,i+1,j+1,i+1,j));

            displayColor(terrain->z(i,j+1));
            glVertex3f(
                       terrain->x(i,j+1),
                       terrain->y(i,j+1),
                       terrain->z(i,j+1));
            //id = (i+1)*terrain->getWidth() +j+1;
            displayColor(terrain->z(i,j));
            glVertex3f(
                       terrain->x(i+1,j+1),
                       terrain->y(i+1,j+1),
                       terrain->z(i+1,j+1));
            //id = (i+1)*terrain->getWidth() +j;
            displayColor(terrain->z(i,j));
            glVertex3f(
                       terrain->x(i+1,j),
                       terrain->y(i+1,j),
                       terrain->z(i+1,j));
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


void GameWindow::updateParticlesAut()
{
    //int id2;
#pragma omp parallel
    {
#pragma omp for
        for(int id = 0; id < numParticules; id++)
        {
            particules[id].z -= 0.0003f * ((float) minP + (rand() % (int)(maxP - minP + 1)));
            //id2 = (particules[id].x)*terrain->getWidth() + particules[id].y;
            if(particules[id].z < terrain->z(particules[id].x,particules[id].y))
            {
                int angle =minP + (rand() % (int)(maxP - minP + 1));
                int dist = (rand() % (int)(100 ));
                int alt = (rand() % (int)(100));
                float x = sin(
                              ((3.14159 * 2) *
                               angle
                               )/360
                              )*dist;
                float y = cos(
                              ((3.14159 * 2) *
                               angle
                               )/360
                              )*dist;

                particules[id].x = 0.5f + (float)+x/(terrain->getWidth()) - ((float)terrain->getWidth()/2.0)/terrain->getWidth();
                particules[id].y = 0.5f + (float)y/(terrain->getHeight()) - ((float)terrain->getHeight()/2.0)/terrain->getHeight();
                particules[id].z = (float)(alt)/100;

            }
        }
    }

    glColor3f(0.2f, 0.2f, 1.0f);
    glPointSize(0.01f);
    glBegin(GL_POINTS);
    for(int id = 0; id < numParticules; id++)
    {
        glVertex3f(
                   particules[id].x,
                   particules[id].y,
                   particules[id].z);


    }
    glEnd();
}

void GameWindow::displayQuadTree()
{
    glColor3f(1.0f, 1.0f, 1.0f);
//    for(int i = 0 ; i < quadset_size-1;i+=3) {
//        glBegin(GL_TRIANGLES);

//        displayColor(terrain->z(quadset_indices[i]));
//        //glNormal3fv(terrain->normal(i,j,i,j+1,i+1,j));
//        glVertex3f(
//                   terrain->x(quadset_indices[i]),
//                   terrain->y(quadset_indices[i]),
//                   terrain->z(quadset_indices[i]));
//        //id = i*terrain->getWidth() +(j+1);
//        displayColor(terrain->z(quadset_indices[i+1]));
//        glVertex3f(
//                   terrain->x(quadset_indices[i+1]),
//                   terrain->y(quadset_indices[i+1]),
//                   terrain->z(quadset_indices[i+1]));
//        //id = (i+1)*terrain->getWidth() +j;
//        displayColor(terrain->z(quadset_indices[i+2]));
//        glVertex3f(
//                   terrain->x(quadset_indices[i+2]),
//                   terrain->y(quadset_indices[i+2]),
//                   terrain->z(quadset_indices[i+2]));

//        displayColor(terrain->z(quadset_indices[i+1]));
////        glVertex3f(
////                   terrain->x(quadset_indices[i+2]),
////                   terrain->y(quadset_indices[i+2]),
////                   terrain->z(quadset_indices[i+2]));

////        glVertex3f(
////                   terrain->x(quadset_indices[i+1]),
////                   terrain->y(quadset_indices[i+1]),
////                   terrain->z(quadset_indices[i+1]));
////        //id = i*terrain->getWidth() +(j+1);
////        displayColor(terrain->z(quadset_indices[i+3]));
////        glVertex3f(
////                   terrain->x(quadset_indices[i+3]),
////                   terrain->y(quadset_indices[i+3]),
////                   terrain->z(quadset_indices[i+3]));
//        glEnd();
//    }
    qt->draw();
}

void GameWindow::updateParticlesHiv()
{
    //int id2;
#pragma omp parallel
    {
#pragma omp for
        for(int id = 0; id < numParticules; id++)
        {
            particules[id].z -= 0.00001f * ((float) minP + (rand() % (int)(maxP - minP + 1)));
            //id2 = (particules[id].x)*terrain->getWidth() + particules[id].y;
            if(particules[id].z < terrain->z(particules[id].x,particules[id].y))
            {
                int angle =minP + (rand() % (int)(maxP - minP + 1));
                int dist = (rand() % (int)(100 ));
                int alt = (rand() % (int)(100));
                float x = sin(
                              ((3.14159 * 2) *
                               angle
                               )/360
                              )*dist;
                float y = cos(
                              ((3.14159 * 2) *
                               angle
                               )/360
                              )*dist;

                particules[id].x = 0.5f + (float)+x/(terrain->getWidth()) - ((float)terrain->getWidth()/2.0)/terrain->getWidth();
                particules[id].y = 0.5f + (float)y/(terrain->getHeight()) - ((float)terrain->getHeight()/2.0)/terrain->getHeight();
                particules[id].z = (float)(alt)/100;
            }
        }
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(0.0001f);
    glBegin(GL_POINTS);
    for(int id = 0; id < numParticules; id++)
    {
        glVertex3f(
                   particules[id].x,
                   particules[id].y,
                   particules[id].z);


    }
    glEnd();
}
