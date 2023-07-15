#include "ce_bitmap.h"

CEBitmap::CEBitmap() {
   this->width = 0;
   this->height = 0;
   this->sizeMultiplier = 1;
   this->buffer = NULL;
   this->addCurrentWindowAsAltered();
   this->transparent = false;
}

void CEBitmap::setTransparent(bool transparent) {
    this->transparent = transparent;
    this->addCurrentWindowAsAltered();
}

bool CEBitmap::isTransparent() {
    return this->transparent;
}

void CEBitmap::setWidth(unsigned int w) {
    this->width = w;
    this->addCurrentWindowAsAltered();
}

void CEBitmap::setHeight(unsigned int h) {
    this->height = h;
    this->addCurrentWindowAsAltered();
}

void CEBitmap::setSizeMultiplier(uint8_t size) {
    this->sizeMultiplier = size;
    this->addCurrentWindowAsAltered();
}

uint8_t CEBitmap::getSizeMultiplier() {
    return this->sizeMultiplier;
}

unsigned int CEBitmap::getWidth() {
    return this->width * this->sizeMultiplier;
}

unsigned int CEBitmap::getHeight() {
    return this->height * this->sizeMultiplier;;
}

CERenderWindow* CEBitmap::getRenderWindow() {
    CERenderWindow *window = this->getDefaultRenderWindow();
    CEPoint *start = new CEPoint((int) this->posX, (int) this->posY);
    CEPoint *end = new CEPoint((int) (this->posX + this->getWidth()), (int) (this->posY + this->getHeight()));
    window->setPoints(start, end);
    delete start;
    delete end;
    return window;
}

void CEBitmap::setBuffer(const uint8_t *buffer) {
    this->buffer = buffer;
    this->addCurrentWindowAsAltered();
}

const uint8_t *CEBitmap::getBuffer() {
    return this->buffer;
}

bool CEBitmap::renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow) {
    int startX = window->topLeft->x > subWindow->topLeft->x ? window->topLeft->x : subWindow->topLeft->x;
    int startY = window->topLeft->y > subWindow->topLeft->y ? window->topLeft->y : subWindow->topLeft->y;
    int endX = window->bottomRight->x < subWindow->bottomRight->x ? window->bottomRight->x : subWindow->bottomRight->x;
    int endY = window->bottomRight->y < subWindow->bottomRight->y ? window->bottomRight->y : subWindow->bottomRight->y;

    unsigned int cursorY = startY;
    unsigned int internalCursorY = 0;
    bool allPixelRendered = true;
    while(cursorY < endY) {
        unsigned int cursorX = startX;
        unsigned int internalCursorX = 0;
        while (cursorX < endX)
        {
            unsigned int bitPos = internalCursorX + (internalCursorY * this->width);
            unsigned int bytePos = bitPos / 8;
            unsigned int bitOffset = bitPos % 8;
            for (size_t i = 0; i < this->getSizeMultiplier(); i++) {
                for (size_t j = 0; j < this->getSizeMultiplier(); j++) {
                    bool bitSet = (bool) (this->buffer[bytePos] & (1 << (7 - bitOffset)));
                    if(!bitSet && this->transparent) {
                        continue;
                    }
                    if(
                        !this->setPixel(
                            canvas,
                            i + cursorX,
                            j + cursorY,
                            bitSet ? this->getBaseColor() : canvas->getBackgroundColor()
                        )
                    ) {
                        allPixelRendered = false;
                    }
                }
            }
            cursorX += this->getSizeMultiplier();
            internalCursorX++;
        }
        cursorY += this->getSizeMultiplier();
        internalCursorY++;
    }
    
    return allPixelRendered;
}
