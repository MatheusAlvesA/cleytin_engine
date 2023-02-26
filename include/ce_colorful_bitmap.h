#ifndef CE_COLORFUL_BITMAP_H
#define CE_COLORFUL_BITMAP_H

#include "cleytin_engine.h"

class CEColorfulBitmap : public CEGraphicObject {
public:
    CEColorfulBitmap();
    ~CEColorfulBitmap();
    void setWidth(unsigned int w);
    void setHeight(unsigned int h);
    void setBuffer(uint16_t *buffer);
    void setSizeMultiplier(uint8_t size);
    unsigned int getWidth();
    void setAlphaColor(uint16_t color);
    unsigned int getHeight();
    uint16_t getAlphaColor();
    uint16_t *getBuffer();
    uint8_t getSizeMultiplier();

    CERenderWindow* getRenderWindow();
    bool renderToCanvas(CECanvas *canvas, CERenderWindow *window);

protected:
    unsigned int width;
    unsigned int height;
    uint16_t *buffer;
    uint16_t alphaColor;
    uint8_t sizeMultiplier;

    CEColor rgb565ToColor(const uint16_t raw);
};

#endif
