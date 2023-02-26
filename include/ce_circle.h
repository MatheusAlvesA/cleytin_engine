#ifndef CE_CIRCLE_H
#define CE_CIRCLE_H

#include "cleytin_engine.h"

class CECircle : public CEGraphicObject {
public:
    CECircle();
    void setRadius(unsigned int r);
    void setFilled(bool fill);
    void setRotation(uint16_t rotation);

    unsigned int getRadius();
    bool getFilled();
    virtual uint16_t getRotation();

    CERenderWindow* getRenderWindow();
    bool renderToCanvas(CECanvas *canvas, CERenderWindow *window);

protected:
    unsigned int radius;
    bool filled;
};
#endif
