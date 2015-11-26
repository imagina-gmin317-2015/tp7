#include "quadtree.h"
#include <QDebug>
using namespace std;

int* QuadTree_func(Terrain* t, int* indices, float tolerance, int* current, int min_width, int min_height, int max_width, int max_height) {

    if(indices == nullptr) {
        indices = new int[250000];
        for (int i=0;i<250000;++i)
            indices[i]=0;
    }
    if(current == nullptr) {
        current = new int[1];
        current = 0;
    }

    if(min_width < max_width -1 && min_height < max_height -1){
        float max1 = std::max(t->z(min_width,min_height), t->z(max_width,min_height));
        float max2 = max(max1, t->z(min_width,max_height));
        float maxF = max(max2, t->z(max_width,max_height));

        float min1 = min(t->z(min_width,min_height), t->z(max_width,min_height));
        float min2 = min(min1, t->z(min_width,max_height));
        float minF = min(min2, t->z(max_width,max_height));

        if(maxF-minF < 0.1f * tolerance){
            int k = *current;
            qDebug() << k;
            indices[k++] = t->coord(min_width,min_height);
            indices[k++] = t->coord(max_width,min_height);
            indices[k++] = t->coord(min_width,max_height);
            indices[k++] = t->coord(max_width,max_height);
            current[0]=k;
        }else{
            QuadTree_func(t, indices, tolerance,current, min_width, min_height,(max_width+min_width)/2, (max_height+min_height)/2);
            QuadTree_func(t, indices, tolerance,current, (max_width+min_width)/2, min_height,max_width, (max_height+min_height)/2);
            QuadTree_func(t, indices, tolerance,current, min_width, (max_height+min_height)/2,(max_width+min_width)/2, max_height);
            QuadTree_func(t, indices, tolerance,current, (max_width+min_width)/2, (max_height+min_height)/2,max_width, max_height);
        }
    }else{
        int k = *current;
        indices[k++] = t->coord(min_width,min_height);
        indices[k++] = t->coord(max_width,min_height);
        indices[k++] = t->coord(min_width,max_height);
        indices[k++] = t->coord(max_width,max_height);
        current[0]=k;
    }
    return indices;
}

