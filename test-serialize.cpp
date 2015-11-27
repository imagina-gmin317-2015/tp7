#include <QCoreApplication>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QMap>
#include <QDebug>

void Save()
{
    QString filename = "C:/Qt/Test/serial.txt";
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open " << filename;
        return;
    }

    int MyPass = 999;
    QMap<int,QString> map;
    map.insert(1,"uno");
    map.insert(2,"dos");
    map[3] = "tres";

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);

    out << MyPass << map;

    file.flush();
    file.close();
}

void Load()
{
    QString filename = "C:/Qt/Test/serial.txt";
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open " << filename;
        return;
    }

    int MyPass;
    QMap<int,QString> map;

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_1);

    in >> MyPass >> map;

    file.close();

    qDebug() << "MyPass = " << MyPass;
    foreach(QString item, map.values())
        qDebug() << item;
}
