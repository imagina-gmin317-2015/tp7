#include "filemanager.h"
#include <QFile>
#include <QDataStream>
#include "terrain.h"

FileManager::FileManager()
{
}

FileManager::~FileManager() {

}

void FileManager::save()
{
    QFile file("./tp4.bin");
    if(!file.open(QIODevice::WriteOnly)) {
        return ;
    }
    QDataStream stream(&file);
    stream << windows.size();
    foreach(GameWindow* win, windows) {
        paramCamera* c0 = win->c;
        stream << c0->anim;
        stream << c0->etat;
        stream << c0->rotX;
        stream << c0->rotY;
        stream << c0->ss;

         QString season;
         if(win->season == 0)
             season+="Spring";
         else if(win->season == 1)
             season+="Summer";
         else if(win->season == 2)
             season+="Autumn";
         else
             season+="Winter";
         stream << season;
         win->terrain->serialize(stream);

    }

    file.close();

}

void FileManager::link(GameWindow *win)
{
    windows.append(win);
}

QVector<GameWindow*> FileManager::load()
{
    QFile file("./tp4.bin");
    QVector<GameWindow*> rlt;

    if(!file.open(QIODevice::ReadOnly)) {
        return rlt;
    }

    QDataStream stream(&file);
    int nb=0;

    stream >> nb;qDebug() << nb;
    for(int i=0;i<nb;++i) {
        GameWindow* window = new GameWindow();
        //First load camera
        paramCamera* c0 = new paramCamera;
        stream >> c0->anim;
        stream >> c0->etat;
        stream >> c0->rotX;
        stream >> c0->rotY;
        stream >> c0->ss;
        //Then load Season
        QString season;
        stream >> season;
        window->c = c0;
        if(season.startsWith("Winter"))
            window->setSeason(3);
        else if(season.startsWith("Summer"))
            window->setSeason(1);
        else if(season.startsWith("Spring"))
            window->setSeason(0);
        else
            window->setSeason(2);
        //Unserialize Terrain
        Terrain* terrain = Terrain::unserialize(stream);
        window->setTerrain(terrain);

        rlt.append(window);
    }
    file.close();

    return rlt;
}

FileManager& FileManager::getInstance()
{
    static FileManager instance;
    return instance;
}
