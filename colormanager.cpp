#include "colormanager.h"

ColorManager::ColorManager()
{
    this->colorPreset[0]= new QColor(Qt::red);
    this->colorPreset[1]= new QColor(Qt::green);
    this->colorPreset[2]= new QColor(Qt::blue);
    this->colorPreset[3]= new QColor(Qt::yellow);
    this->colorPreset[4]= new QColor(Qt::white);
}

QColor ColorManager::getColor(int index)
{
    if(index < NUMBER_OF_COLORS){
        return *colorPreset[index];
    } else {
        return getRandomColor();
    }
}

QColor ColorManager::getRandomColor()
{
    return QColor(qrand() % 255, qrand() % 255, qrand() % 255);
}
