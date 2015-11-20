#ifndef VOXEL_H
#define VOXEL_H

#include <QVector3D>

class Voxel
{
    public:
        Voxel();
        Voxel(QVector3D _position, float _length);

        QVector3D getPosition() const { return position; };
        void DisplayVoxel();

        //GETTER
        int getLength() const { return length; };
        int getOffsetX() const { return offsetX; };
        int getOffsetY() const { return offsetY; };

        //SETTER
        void set(QVector3D _position, float _length);
        void setOffset(int offX, int offY) { offsetX = offX; offsetY = offY; };

    private:
        QVector3D position;

        int offsetX;
        int offsetY;
        float length;
};

#endif // VOXEL_H
