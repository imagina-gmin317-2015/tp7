#include "quadtree.h"

Quadtree::Quadtree()
{

}

Quadtree::Quadtree(float xMin, float yMin, float xMax, float yMax, QVector<PlyLoader *> mods, int prof){

    bool div = false;
    this->xMin = xMin;
    this->yMin = yMin;
    this->xMax = xMax;
    this->yMax = yMax;
    this->fNO = nullptr;
    this->fNE = nullptr;
    this->fSO = nullptr;
    this->fSE = nullptr;
    this->profondeur = prof;
    if(prof == 4){
        isFeuille = true;
        for(int i = 0; i< mods.size(); i++){
                if(this->contient(mods.at(i))){
                    this->object.push_back(mods.at(i));
                }
        }
        //qDebug()<<"fin";
    }else{

        for(int i = 0; i< mods.size(); i++){
                if(this->contient(mods.at(i))){
                    this->object.push_back(mods.at(i));
                    div = true;
                    //divise(xMin,zMin, xMax, zMax,mods,prof);
                }
        }
        if(div){
            divise(xMin,yMin, yMax, yMax,this->object,prof);
        }else{
            isNull = true;
        }
    }



}

bool Quadtree::contient(PlyLoader *ply)
{

    QRectF rect = ply->getBounding();
    return rect.intersects(QRectF(QPointF(this->xMin,this->yMin),QPointF(this->xMax,this->yMax)));

/*    QVector<float> bound = ply->getBounding();
     qDebug()<<"contient : "<<bound.at(0)<<" "<<bound.at(1)<<" "<<bound.at(2)<<" "<<bound.at(3)<< " | "<<this->xMin<<" "<<this->yMin<<" "<<this->xMax<<" "<<this->yMax;
    // vrai si le coin superieur droit de ply appartient au quadtree
    if((bound.at(0)> this->xMin && bound.at(0)<this->xMax) && (bound.at(1)> this->yMax && bound.at(1) < this->yMin)){
            return true;
    }

    // vrai si le coin superieur droit de ply appartient au quadtree
    if((bound.at(2)> this->xMin && bound.at(2)<this->xMax) && (bound.at(1)> this->yMax && bound.at(1) < this->yMin)){
        return true;
    }

    // vrai si le coin inferieur gauche de ply appartient au quadtree
    if((bound.at(0)> this->xMin && bound.at(0)<this->xMax) && (bound.at(3)> this->yMax && bound.at(3) < this->yMin)){
        return true;
    }

    // vrai si le coin inferieur droit de ply appartient au quadtree
    if((bound.at(2)> this->xMin && bound.at(2)<this->xMax) && (bound.at(3)> this->yMax && bound.at(3) < this->yMin)){
        return true;
    }*/

    // faux sinon

    return false;

}

void Quadtree::divise(float xMin, float yMin, float xMax, float yMax, QVector<PlyLoader *> mods, int prof){

    // rectangle superieur gauche

    this->fNO = new Quadtree(xMin, yMin,
                             xMin + (xMax - xMin)/2, yMin + (yMax - yMin)/2,
                             mods, prof+1);
    // rectangle superieur droit
    this->fNE = new Quadtree(xMin + (xMax - xMin)/2, yMin,
                             xMax, yMin + (yMax - yMin)/2,
                             mods, prof+1);
   // rectangle inferieur gauche
    this->fSO = new Quadtree(xMin, yMin + (yMax - yMin)/2,
                             xMin + (xMax - xMin)/2, yMax,
                             mods,prof+1);
     // rectangle inferieur droit
    this->fSE = new Quadtree(xMin + (xMax - xMin)/2, yMin + (yMax - yMin)/2,
                             xMax, yMax,
                             mods,prof+1);

}

void Quadtree::affiche()
{
    qDebug()<<"affichage "<<this->profondeur;
    glPointSize(2.0f);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(this->xMin,this->yMin,0.5f);
    glVertex3f(this->xMax,this->yMin,0.5f);
    glVertex3f(this->xMax,this->yMax,0.5f);
    glVertex3f(this->xMin,this->yMax,0.5f);
    glVertex3f(this->xMin,this->yMin,0.5f);
    glEnd();
    glColor3f(0.5,0.5,0.5);
    glDisable(GL_COLOR_MATERIAL);

    if(this->fNO != nullptr){
        this->fNO->affiche();
    }
    if(this->fNE != nullptr){
        this->fNE->affiche();
    }
    if(this->fSO != nullptr){
        this->fSO->affiche();
    }
    if(this->fSE != nullptr){
        this->fSE->affiche();
    }
}

void Quadtree::getPly()
{
    if(!this->isNull){
        if(this->isFeuille){
            qDebug()<<this->object.at(0)->getName();
        }else{
            this->fNO->getPly();
            this->fNE->getPly();
            this->fNE->getPly();
            this->fNE->getPly();
        }
    }
}
