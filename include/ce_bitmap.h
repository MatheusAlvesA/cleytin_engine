#ifndef CE_BITMAP_H
#define CE_BITMAP_H

#include "cleytin_engine.h"

class CEBitmap : public CEGraphicObject {
public:
    CEBitmap();
    void setWidth(unsigned int w);
    void setHeight(unsigned int h);
    void setBuffer(const uint8_t *buffer);
    void setTransparent(bool transparent);
    void setSizeMultiplier(uint8_t size);
    unsigned int getWidth();
    unsigned int getHeight();
    const uint8_t *getBuffer();
    uint8_t getSizeMultiplier();
    bool isTransparent();

    CERenderWindow* getRenderWindow();
    bool renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow);

protected:
    unsigned int width;
    unsigned int height;
    const uint8_t *buffer;
    uint8_t sizeMultiplier;
    bool transparent;
};

#endif
