#ifndef CAMERA_H
#define CAMERA_H

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>


class Camera{
 public:
    Camera();

    int getEtat();
    float getRotX();
    float getRotY();
    float getSs();

    void setEtat(int);
    void setRotX(float);
    void setRotY(float);
    void setSs(float);


    void render();


private:
    int etat;
    float rotX;
    float rotY;
    float ss;
};


#endif // CAMERA_H

