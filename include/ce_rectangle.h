#ifndef CE_RECTANGLE_H
#define CE_RECTANGLE_H

#include "cleytin_engine.h"

class CERectangle : public CEGraphicObject {
public:
    CERectangle();
    void setWidth(unsigned int w);
    void setHeight(unsigned int h);
    void setPosX(int x);
    void setPosY(int y);
    void setPos(int x, int y);
    void setFilled(bool fill);
    bool getFilled();
    unsigned int getWidth();
    unsigned int getHeight();

    CERenderWindow* getRenderWindow();
    void renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow);

protected:
    unsigned int width;
    unsigned int height;
    bool filled;
};

#endif
