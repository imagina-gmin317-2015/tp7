#include "camera.h"


Camera::Camera(){
    etat = 1;
    ss = 1.0f;
    rotX = -45.0f;
    rotY = -45.0;
}


int Camera::getEtat(){
    return etat;
}

float Camera::getRotX(){
    return rotX;
}

float Camera::getRotY(){
    return rotY;
}

float Camera::getSs(){
    return ss;
}


void Camera::setEtat(int i){
    etat = i;
}

void Camera::setRotX(float f){
    rotX = f;
}

void Camera::setRotY(float f){
    rotY = f;
}

void Camera::setSs(float f){
    ss = f;
}


void Camera::render(){
    glScalef(ss,ss,ss);
    glRotatef(rotX,1.0f,0.0f,0.0f);
    glRotatef(rotY,0.0f,0.0f,1.0f);
}
