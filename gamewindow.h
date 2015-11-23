#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "openglwindow.h"
#include "camera.h"
#include <vector>
#include <iterator>
using namespace std;

struct point
{
    float x, y ,z;
};




class GameWindow : public OpenGLWindow
{
public:
    GameWindow();
    GameWindow(int);

    void initialize();
    void render();
    bool event(QEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void displayTriangles();
    void displayLines();
    void displayLinesQuadTree();
    void displayTrianglesC();
    void displayPoints();
    void displayPointsQuadTree();
    void displayTrianglesTexture();

    void displayColor(float);

    void loadMap(QString localPath);
    void loadMapQuadTree(QString localPath, float tolerance);
    void quadTree(QImage m_image, float tolerance,
                           int widthMin, int widthMax, int heightMin, int heightMax);


    int getFps();

    void setCam(Camera*);

private:

    float const zoom = 0.1f;
    float const rotation = 1.0f;

    QTimer *timer;
    int fps;
    int m_frame;
    QImage m_image;
    point *p;

    int **relationsQT;
    int *cptQT;

    int carte=1;

    Camera *cam;
    bool tourniquet = false;

};


#endif // GAMEWINDOW_H
