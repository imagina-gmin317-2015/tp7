#ifndef QUADTREE_H
#define QUADTREE_H
#include "terrain.h"

#include <algorithm>
#include <QList>

using namespace std;

int* QuadTree_func(Terrain* t, int* indices,float tolerance,int* current, int min_width, int min_height, int max_width, int max_height);

class QuadTree {
private:
    Terrain* m_terrain;
    QList<QuadTree*> m_children;
    int m_min_width;
    int m_min_height;
    int m_max_width;
    int m_max_height;

public:
    QuadTree(Terrain* t) {
        m_terrain = t;
        m_min_width = 0;
        m_min_height = 0;
        m_max_width = t->getWidth();
        m_max_height = t->getHeight();
    }

    QuadTree(Terrain* t, int min_width, int min_height, int max_width, int max_height) {
        m_terrain = t;
        m_min_width = min_width;
        m_min_height = min_height;
        m_max_width = max_width;
        m_max_height = max_height;
    }
    virtual void clear() {
        while(!m_children.empty()) {
            QuadTree* qt = m_children.back();
            m_children.pop_back();
            delete qt;
        }
    }



    virtual ~QuadTree() {
        clear();
    }


    bool haveChildren() {
        return m_children.size()>0;
    }

    virtual void makeChildren() {
        int mid_width = (m_max_width+m_min_width)/2;
        int mid_height = (m_max_height+m_min_height)/2;


        QuadTree* tl = new QuadTree(m_terrain,m_min_width,m_min_height,mid_width,mid_height);
        QuadTree* tr = new QuadTree(m_terrain,mid_width,m_min_height,m_max_width,mid_height);
        QuadTree* dl = new QuadTree(m_terrain,m_min_width,mid_height,mid_width,m_max_height);
        QuadTree* dr = new QuadTree(m_terrain,mid_width,mid_height,m_max_width,m_max_height);
        m_children.append(tl);
        m_children.append(tr);
        m_children.append(dl);
        m_children.append(dr);
    }

    virtual void tolerate(float tolerance, float min_patch_size=1) {
        if (((m_max_width - m_min_width) < min_patch_size)
                || (m_max_height - m_min_height) < min_patch_size)
        {
            clear();
            return;
        }

        if(! (m_min_width < m_max_width -1 || m_min_height < m_max_height -1)){

            clear();
            return;
        }
        float max1 = std::max(m_terrain->z(m_min_width,m_min_height), m_terrain->z(m_max_width-1,m_min_height));
        float max2 = max(max1, m_terrain->z(m_min_width,m_max_height-1));
        float maxF = max(max2, m_terrain->z(m_max_width-1,m_max_height-1));

        float min1 = min(m_terrain->z(m_min_width,m_min_height), m_terrain->z(m_max_width-1,m_min_height));
        float min2 = min(min1, m_terrain->z(m_min_width,m_max_height-1));
        float minF = min(min2, m_terrain->z(m_max_width-1,m_max_height-1));

        if(maxF-minF < tolerance){
            if(!m_children.empty())
                m_children.clear();
        }else{
            if(m_children.empty())
                makeChildren();
            foreach(QuadTree* qt, m_children) {
                qt->tolerate(tolerance, min_patch_size);
            }

        }

    }

    virtual void draw() {
        if(haveChildren()) {
            foreach(QuadTree* qt,m_children)
                qt->draw();
            return;

            int max_height = (m_terrain->getHeight()==m_max_height) ? m_max_height-1 : m_max_height;
            int max_width = (m_terrain->getWidth()==m_max_width) ? m_max_width-1 : m_max_width;
            glBegin(GL_TRIANGLES);
            //displayColor(m_terrain->z(m_min_width,m_min_height));
            glNormal3fv(m_terrain->normal(m_min_width,m_min_height,m_min_width,max_height,max_width,m_min_height));
            glVertex3f(
                       m_terrain->x(m_min_width,m_min_height),
                       m_terrain->y(m_min_width,m_min_height),
                       m_terrain->z(m_min_width,m_min_height));
            //id = i*terrain->getWidth() +(j+1);
            //displayColor(m_terrain->z(m_min_width,m_max_height));

            glVertex3f(
                       m_terrain->x(m_min_width,max_height),
                       m_terrain->y(m_min_width,max_height),
                       m_terrain->z(m_min_width,max_height));
            //id = (i+1)*terrain->getWidth() +j;
            //displayColor(m_terrain->z(m_max_width,m_min_height));
            glVertex3f(
                       m_terrain->x(max_width,m_min_height),
                       m_terrain->y(max_width,m_min_height),
                       m_terrain->z(max_width,m_min_height));


            glNormal3fv(m_terrain->normal(m_min_width,max_height,max_width,m_min_height,max_width,max_height));
            glVertex3f(
                       m_terrain->x(m_min_width,max_height),
                       m_terrain->y(m_min_width,max_height),
                       m_terrain->z(m_min_width,max_height));
            //id = (i+1)*terrain->getWidth() +j;
            //displayColor(m_terrain->z(m_max_width,m_min_height));
            glVertex3f(
                       m_terrain->x(max_width,m_min_height),
                       m_terrain->y(max_width,m_min_height),
                       m_terrain->z(max_width,m_min_height));

            glVertex3f(
                       m_terrain->x(max_width,max_height),
                       m_terrain->y(max_width,max_height),
                       m_terrain->z(max_width,max_height));
            glEnd();
    }

    }
};


#endif // QUADTREE_H
