#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>

#include "voxel.h"

Voxel::Voxel()
{
    position = QVector3D(0,0,0);
    length = 1;
}

Voxel::Voxel(QVector3D _position, float _length)
{
    position = QVector3D(_position.x(),_position.y(),_position.z());
    length = _length;
}

void Voxel::set(QVector3D _position, float _length){
    position = QVector3D(_position.x(),_position.y(),_position.z());
    length = _length;
}

void Voxel::DisplayVoxel(){
    glPushMatrix();
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(position.x(), position.y(), position.z());

    glColor3f(0.f,0.f,1.f);
    float midLength = length / 2.f;
    float yLength = 5.f;

    glCullFace(GL_FRONT);
    glBegin(GL_QUADS);
        glVertex3f(position.x() - midLength, position.y() - yLength, position.z() + midLength);
        glVertex3f(position.x() + midLength, position.y() - yLength, position.z() + midLength);
        glVertex3f(position.x() + midLength, position.y() + yLength , position.z() + midLength);
        glVertex3f(position.x() - midLength, position.y() + yLength , position.z() + midLength);

        glVertex3f(position.x() + midLength, position.y() - yLength , position.z() - midLength);
        glVertex3f(position.x() - midLength, position.y() - yLength , position.z() - midLength);
        glVertex3f(position.x() - midLength, position.y() + yLength , position.z() - midLength);
        glVertex3f(position.x() + midLength, position.y() + yLength , position.z() - midLength);

        glVertex3f(position.x() - midLength, position.y() - yLength , position.z() - midLength);
        glVertex3f(position.x() - midLength, position.y() - yLength , position.z() + midLength);
        glVertex3f(position.x() - midLength, position.y() + yLength , position.z() + midLength);
        glVertex3f(position.x() - midLength, position.y() + yLength , position.z() - midLength);

        glVertex3f(position.x() + midLength, position.y() - yLength , position.z() + midLength);
        glVertex3f(position.x() + midLength, position.y() - yLength , position.z() - midLength);
        glVertex3f(position.x() + midLength, position.y() + yLength , position.z() - midLength);
        glVertex3f(position.x() + midLength, position.y() + yLength , position.z() + midLength);
    glEnd();

    glCullFace(GL_BACK);
    glBegin(GL_QUADS);
        glVertex3f(position.x() - midLength, position.y() - yLength , position.z() + midLength);
        glVertex3f(position.x() + midLength, position.y() - yLength , position.z() + midLength);
        glVertex3f(position.x() + midLength, position.y() + yLength , position.z() + midLength);
        glVertex3f(position.x() - midLength, position.y() + yLength , position.z() + midLength);

        glVertex3f(position.x() + midLength, position.y() - yLength , position.z() - midLength);
        glVertex3f(position.x() - midLength, position.y() - yLength , position.z() - midLength);
        glVertex3f(position.x() - midLength, position.y() + yLength , position.z() - midLength);
        glVertex3f(position.x() + midLength, position.y() + yLength , position.z() - midLength);

        glVertex3f(position.x() - midLength, position.y() - yLength , position.z() - midLength);
        glVertex3f(position.x() - midLength, position.y() - yLength , position.z() + midLength);
        glVertex3f(position.x() - midLength, position.y() + yLength , position.z() + midLength);
        glVertex3f(position.x() - midLength, position.y() + yLength , position.z() - midLength);

        glVertex3f(position.x() + midLength, position.y() - yLength , position.z() + midLength);
        glVertex3f(position.x() + midLength, position.y() - yLength , position.z() - midLength);
        glVertex3f(position.x() + midLength, position.y() + yLength , position.z() - midLength);
        glVertex3f(position.x() + midLength, position.y() + yLength , position.z() + midLength);
    glEnd();

    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}
