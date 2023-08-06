#ifndef CE_COLORFUL_BITMAP_H
#define CE_COLORFUL_BITMAP_H

#include "cleytin_engine.h"

class CEColorfulBitmap : public CEGraphicObject {
public:
    CEColorfulBitmap();
    void setWidth(unsigned int w);
    void setHeight(unsigned int h);
    void setBuffer(const uint16_t *buffer);
    void setSizeMultiplier(uint8_t size);
    void setHasTransparency(bool hasTransparency);
    unsigned int getWidth();
    void setAlphaColor(uint16_t color);
    unsigned int getHeight();
    uint16_t getAlphaColor();
    const uint16_t *getBuffer();
    uint8_t getSizeMultiplier();

    CERenderWindow* getRenderWindow();
    bool renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow);
    bool fastRenderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow);

protected:
    unsigned int width;
    unsigned int height;
    const uint16_t *buffer;
    uint16_t alphaColor;
    uint8_t sizeMultiplier;
    bool hasTransparency;
};

#endif
