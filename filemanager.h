#include "gameobject.h"
#include "gamewindow.h"

#include <QString>
#include <QFile>

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class FileManager
{
private:
    FileManager();
    QVector<GameWindow*> windows;
    ~FileManager();

public:
    static FileManager& getInstance();

    void save();

    void link(GameWindow*);

    bool haveBigFile() {
        return QFile::exists("./tp4.bin");
    }

    virtual QVector<GameWindow*> load();


};

#endif // FILEMANAGER_H
