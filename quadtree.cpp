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
    this->profondeur = prof;
    if(prof == 20){
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
                    qDebug()<<"add";
                    div = true;
                    //divise(xMin,zMin, xMax, zMax,mods,prof);
                }
        }
        if(div){
            divise(xMin,yMin, yMax, yMax,mods,prof);
        }else{
            isNull = true;
        }
    }



}

bool Quadtree::contient(PlyLoader *ply)
{

    QVector<float> bound = ply->getBounding();
     //qDebug()<<"contient : "<<bound.at(0)<<" "<<bound.at(1)<<" "<<bound.at(2)<<" "<<bound.at(3)<< " | "<<this->xMin<<" "<<this->yMin<<" "<<this->xMax<<" "<<this->yMax;
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
    }

    // faux sinon

    return false;

}

void Quadtree::divise(float xMin, float yMin, float xMax, float yMax, QVector<PlyLoader *> mods, int prof){

    //qDebug()<<"divise";
    this->fNO = new Quadtree(xMin,yMin,(xMax-xMin)/2,(yMax-yMin)/2,mods,prof+1); // rectangle superieur gauche
    this->fNE = new Quadtree((xMax-xMin)/2,yMin,xMax,(yMax-yMin)/2,mods,prof+1); // rectangle superieur droit
    this->fNE = new Quadtree(xMin,(yMax-yMin)/2,(xMax-xMin)/2,yMax,mods,prof+1); // rectangle inferieur gauche
    this->fNE = new Quadtree((xMax-xMin)/2,(yMax-yMin)/2,xMax,yMax,mods,prof+1); // rectangle inferieur droit

}

void Quadtree::affiche()
{
    if(!this->isNull){
            if(this->isFeuille){
                qDebug()<<"objets : "+this->object.size() ;
            }else{
                qDebug()<<"pere de 4 fils";
                this->fNO->affiche();
                this->fNE->affiche();
                this->fNE->affiche();
                this->fNE->affiche();

            }

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
