#include "renderer.h"
#include <QOpenGLFunctions>
#include <QtCore>

Renderer::Renderer(Mesh* mesh, GameObject* parent) : GameObject(parent), m_mesh(mesh)
{

}

void Renderer::draw() {
    glPushMatrix();
    glTranslatef(GameObject::localPosition.x(),GameObject::localPosition.y(),GameObject::localPosition.y());
    glScalef(GameObject::localScale.x(),GameObject::localScale.y(),GameObject::localScale.z());
    glRotatef(GameObject::localRotation.x(),1,0,0);
    glRotatef(GameObject::localRotation.y(),0,1,0);
    glRotatef(GameObject::localRotation.z(),0,0,1);
    #pragma omp for schedule(dynamic)
    for(int z = 0; z < m_mesh->getIndices().size(); ++z) {
        //qDebug() << z+1<<"/"<<m_mesh->getIndices().size();
        QVector<int> face = m_mesh->getIndices().at(z);
        if(face.size()==1)
            glBegin(GL_POINTS);
        if(face.size()==2)
            glBegin(GL_LINES);
        if(face.size()==3)
            glBegin(GL_TRIANGLES);
        if(face.size()==4)
            glBegin(GL_QUADS);
        else
            glBegin(GL_POLYGON);

        //int k = 1;//debug variable
        foreach(int p0, face) {
            //qDebug() << p0 << " on " << m_mesh->getPoints().size() << "("<<k++<<"/"<<face.size()<<")";
            if(p0>m_mesh->getPoints().size())
                break;
            glVertex3f(m_mesh->getPoints().at(p0).x(),
                       m_mesh->getPoints().at(p0).y(),
                       m_mesh->getPoints().at(p0).z());
        }
        glEnd();
    }

    //Draw subnodes
    glPopMatrix();
}
