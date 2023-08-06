#ifndef CE_CANVAS_TFTLCD_320X240_H
#define CE_CANVAS_TFTLCD_320X240_H

#include <stdint.h>
#include <string.h>
#include "ce_canvas.h"
#include "cleytin_tftlcd_api.h"

#define LCD_WIDTH_PIXELS 320
#define LCD_HEIGHT_PIXELS 240

class CECanvasTFTLCD320x240 : public CECanvas {
public:
    CECanvasTFTLCD320x240();
    ~CECanvasTFTLCD320x240();
    unsigned int getCanvasWidth();
    unsigned int getCanvasHeight();
    bool setPixel(unsigned int x, unsigned int y, const CEColor color);
    bool setPixel(unsigned int x, unsigned int y, uint16_t color);
    bool clearPixel(unsigned int x, unsigned int y);
    bool render();
    void clear();
    bool prepareWindow(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY);

private:
    uint16_t *frameBuffer;
    CleytinTFTAPI *lcdAPI;
    uint16_t startX;
    uint16_t startY;
    uint16_t endX;
    uint16_t endY;
    size_t bufferPointer;
};

#endif
