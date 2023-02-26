#ifndef CE_BITMAP_H
#define CE_BITMAP_H

#include "cleytin_engine.h"

class CEBitmap : public CEGraphicObject {
public:
    CEBitmap();
    ~CEBitmap();
    void setWidth(unsigned int w);
    void setHeight(unsigned int h);
    void setBuffer(uint8_t *buffer);
    void setSizeMultiplier(uint8_t size);
    unsigned int getWidth();
    unsigned int getHeight();
    uint8_t *getBuffer();
    uint8_t getSizeMultiplier();

    CERenderWindow* getRenderWindow();
    bool renderToCanvas(CECanvas *canvas, CERenderWindow *window);

protected:
    unsigned int width;
    unsigned int height;
    uint8_t *buffer;
    uint8_t sizeMultiplier;
};

#endif
