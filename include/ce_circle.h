#ifndef CE_CIRCLE_H
#define CE_CIRCLE_H

#include "cleytin_engine.h"

class CECircle : public CEGraphicObject {
public:
    CECircle();
    void setRadius(unsigned int r);
    void setFilled(bool fill);

    unsigned int getRadius();
    bool getFilled();

    CERenderWindow* getRenderWindow();
    void renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow);

protected:
    unsigned int radius;
    bool filled;
};
#endif
