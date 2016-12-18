#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <QColor>

//Можно упороться и сделать правильно, как реализацию интерфейса
class ColorManager
{
public:
    ColorManager();
    QColor getColor(int index);
    QColor getRandomColor();
private:
    const static int NUMBER_OF_COLORS = 5;
    QColor *colorPreset[NUMBER_OF_COLORS];

};

#endif // COLORMANAGER_H
