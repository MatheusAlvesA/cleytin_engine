#ifndef CE_CANVAS_H
#define CE_CANVAS_H

#include <stdint.h>

struct CEColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    bool operator==(const CEColor &a) const {
        return (
            this->red == a.red &&
            this->green == a.green &&
            this->blue == a.blue
        );
    }

    CEColor operator-() const {
        CEColor negative = {0xFF, 0xFF, 0xFF};
        negative.red -= this->red;
        negative.green -= this->green;
        negative.blue -= this->blue;
        return negative;
    }
};

CEColor RGB565ToColor(uint16_t raw);
uint16_t colorToRGB565(const CEColor color);

class CECanvas {
public:
    virtual ~CECanvas() = default;
    void setBackgroundColor(const CEColor color);
    CEColor getBackgroundColor();
    virtual unsigned int getCanvasWidth() = 0;
    virtual unsigned int getCanvasHeight() = 0;
    virtual bool setPixel(unsigned int x, unsigned int y, const CEColor color) = 0;
    virtual bool setPixel(unsigned int x, unsigned int y, uint16_t color) = 0;
    virtual bool setLinePixels(unsigned int y, unsigned int startX, unsigned int endX, const uint16_t *pixels) = 0;
    virtual bool clearPixel(unsigned int x, unsigned int y) = 0;
    virtual bool render() = 0;
    virtual void clear() = 0;
    virtual bool prepareWindow(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY) = 0;

private:
    CEColor backgroundColor;
};

#endif
