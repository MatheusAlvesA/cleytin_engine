#include "ce_bitmap.h"

CEBitmap::CEBitmap() {
   this->width = 0;
   this->height = 0;
   this->sizeMultiplier = 1;
   this->buffer = NULL;
}

CEBitmap::~CEBitmap() {
   delete this->buffer;
}

void CEBitmap::setWidth(unsigned int w) {
    this->width = w;
}

void CEBitmap::setHeight(unsigned int h) {
    this->height = h;
}

void CEBitmap::setSizeMultiplier(uint8_t size) {
    this->sizeMultiplier = size;
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

void CEBitmap::setBuffer(uint8_t *buffer) {
    this->buffer = buffer;
}

uint8_t *CEBitmap::getBuffer() {
    return this->buffer;
}

bool CEBitmap::renderToCanvas(CECanvas *canvas, CERenderWindow *window) {
    int startX = window->topLeft->x;
    int startY = window->topLeft->y;
    int endX = window->bottomRight->x;
    int endY = window->bottomRight->y;

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
                    if(
                        !this->setPixel(
                            canvas,
                            i + cursorX,
                            j + cursorY,
                            (this->buffer[bytePos] & (1 << (7 - bitOffset)))
                                ? this->getBaseColor()
                                : canvas->getBackgroundColor()
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
