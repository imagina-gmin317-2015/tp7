#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "openglwindow.h"
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QSharedPointer>
#include <QMatrix4x4>
#include "terrain.h"
#include "renderer.h"
#include "quadtree.h"

struct point
{
    float x, y ,z;
};

class paramCamera
{
public:
    float rotX = -45.0;
    float rotY = -45.0;
    float ss = 1.0f;
    float anim = 0.0f;

    int etat = 0;
};

class GameWindow : public OpenGLWindow
{
Q_OBJECT

public:
    int season, day;
    Terrain* terrain;
    GameWindow();
    GameWindow(int maj);
    void initialize();
    void initializeLighting();
    void render();
    bool event(QEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void displayTriangles();
    void displayLines();
    void displayTrianglesC();
    void displayPoints();
    void displayTrianglesTexture();
    void displayQuadTree();

    void displayColor(float);
    void updateParticlesAut();
    void updateParticlesHiv();
    paramCamera* c;

    void setSeason(int );
    void setTerrain(Terrain* terrain);

public slots:
    void updateSeason();

private:
    int nbTick = 0;
    int m_frame = 0;

    point* particules;
    bool master = false;

    QVector<Renderer*> renderers;

    int carte=1;
    int maj = 20;

    //Function
    int* quadset_indices=nullptr;
    int quadset_size=0;

    //Object
    QuadTree* qt;
    float tolerate=0.5f;
    int min_patch=10;

    QTimer *timer;
    QTimer *timerFPS;
};



#endif // GameWindow_H
