#include "ce_colorful_bitmap.h"

CEColorfulBitmap::CEColorfulBitmap() {
   this->width = 0;
   this->height = 0;
   this->buffer = NULL;
   this->alphaColor = 0;
   this->sizeMultiplier = 1;
}

CEColorfulBitmap::~CEColorfulBitmap() {
   delete this->buffer;
}

void CEColorfulBitmap::setWidth(unsigned int w) {
    this->width = w;
}

void CEColorfulBitmap::setHeight(unsigned int h) {
    this->height = h;
}

void CEColorfulBitmap::setSizeMultiplier(uint8_t size) {
    this->sizeMultiplier = size;
}

uint8_t CEColorfulBitmap::getSizeMultiplier() {
    return this->sizeMultiplier;
}

void CEColorfulBitmap::setAlphaColor(uint16_t color) {
    this->alphaColor = color;
}

unsigned int CEColorfulBitmap::getWidth() {
    return this->width * this->sizeMultiplier;
}

unsigned int CEColorfulBitmap::getHeight() {
    return this->height * this->sizeMultiplier;
}

uint16_t CEColorfulBitmap::getAlphaColor() {
    return this->alphaColor;
}

CERenderWindow* CEColorfulBitmap::getRenderWindow() {
    CERenderWindow *window = this->getDefaultRenderWindow();
    CEPoint *start = new CEPoint((int) this->posX, (int) this->posY);
    CEPoint *end = new CEPoint((int) (this->posX + this->getWidth()), (int) (this->posY + this->getHeight()));
    window->setPoints(start, end);
    delete start;
    delete end;
    return window;
}

void CEColorfulBitmap::setBuffer(uint16_t *buffer) {
    this->buffer = buffer;
}

uint16_t *CEColorfulBitmap::getBuffer() {
    return this->buffer;
}

bool CEColorfulBitmap::renderToCanvas(CECanvas *canvas, CERenderWindow *window) {
    int startX = window->topLeft->x;
    int startY = window->topLeft->y;
    int endX = window->bottomRight->x;
    int endY = window->bottomRight->y;

    unsigned int cursorY = startY;
    unsigned int internalCursorY = 0;
    bool allPixelsRendered = true;
    while(cursorY < endY) {
        unsigned int cursorX = startX;
        unsigned int internalCursorX = 0;
        while (cursorX < endX) {
            unsigned int index = internalCursorX + (internalCursorY * this->width);
            for (size_t i = 0; i < this->getSizeMultiplier(); i++) {
                for (size_t j = 0; j < this->getSizeMultiplier(); j++) {
                    if(
                        !this->setPixel(
                            canvas,
                            i + cursorX,
                            j + cursorY,
                            (this->buffer[index] != this->alphaColor)
                                ? this->rgb565ToColor(this->buffer[index])
                                : canvas->getBackgroundColor()
                        )
                    ) {
                        allPixelsRendered = false;
                    }
                }
            }
            cursorX += this->getSizeMultiplier();
            internalCursorX++;
        }
        cursorY += this->getSizeMultiplier();
        internalCursorY++;
    }
    
    return allPixelsRendered;
}

CEColor CEColorfulBitmap::rgb565ToColor(const uint16_t raw) {
    CEColor r = {0,0,0};

    r.red = (uint8_t) ((raw >> 11) << 3);      // Primeiros 5 bits são o vermelho
    r.green = (uint8_t) ((raw >> 5) << 2);    // Bits 6 até 11 são o verde
    r.blue = (uint8_t) (raw << 3);            // Bits 12 até 16 são o azul

    return r;
}
