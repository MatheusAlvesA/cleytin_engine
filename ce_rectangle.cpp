#include "ce_rectangle.h"

CERectangle::CERectangle() {
   this->width = 0;
   this->height = 0;
   this->filled = false;
}

void CERectangle::setWidth(unsigned int w) {
    this->width = w;
}

void CERectangle::setHeight(unsigned int h) {
    this->height = h;
}

unsigned int CERectangle::getWidth() {
    return this->width;
}

unsigned int CERectangle::getHeight() {
    return this->height;
}

CERenderWindow* CERectangle::getRenderWindow() {
    CERenderWindow *window = this->getDefaultRenderWindow();
    CEPoint *start = new CEPoint((int) this->posX, (int) this->posY);
    CEPoint *end = new CEPoint((int) (this->posX + this->width), (int) (this->posY + this->height));
    window->setPoints(start, end);
    delete start;
    delete end;
    return window;
}

void CERectangle::setFilled(bool fill) {
    this->filled = fill;
}

bool CERectangle::getFilled() {
    return this->filled;
}

bool CERectangle::renderToCanvas(CECanvas *canvas, CERenderWindow *window) {
    int startX = window->topLeft->x;
    int startY = window->topLeft->y;
    int endX = window->bottomRight->x;
    int endY = window->bottomRight->y;

    unsigned int cursorY = startY;
    bool allPixelRendered = true;
    while(cursorY < endY) {
        unsigned int cursorX = startX;
        while (cursorX < endX)
        {
            if(
                !this->setPixel(
                    canvas,
                    cursorX,
                    cursorY,
                    (this->filled || cursorX == startX || cursorX == (endX - 1) || cursorY == startY || cursorY == (endY - 1))
                        ? this->getBaseColor()
                        : canvas->getBackgroundColor()
                )
            ) {
                allPixelRendered = false;
            }
            cursorX++;
        }
        cursorY++;
    }
    
    return allPixelRendered;
}
