#include "ce_canvas_tftlcd_320x240.h"

CECanvasTFTLCD320x240::CECanvasTFTLCD320x240() {
    this->frameBuffer = new uint16_t[this->getCanvasWidth() * this->getCanvasHeight()];
    this->lcdAPI = new CleytinTFTAPI();
    this->bufferPointer = 0;
    this->setBackgroundColor({0xFF, 0xFF, 0xFF});
    this->prepareWindow(0, 0, this->getCanvasWidth(), this->getCanvasHeight());

    this->render();
}

CECanvasTFTLCD320x240::~CECanvasTFTLCD320x240() {
    delete this->frameBuffer;
    delete this->lcdAPI;
}

bool CECanvasTFTLCD320x240::setPixel(unsigned int x, unsigned int y, const CEColor color) {
    return this->setPixel(x, y, colorToRGB565(color));
}

bool CECanvasTFTLCD320x240::setLinePixels(unsigned int y, unsigned int startX, unsigned int endX, const uint16_t *pixels) {
    if(y > this->endY || y < this->startY || endX < this->startX || startX > this->endX || startX > endX) {
        return false;
    }
    if(endX > this->endX) {
        endX = this->endX;
    }
    unsigned int offset = 0;
    if(startX < this->startX) {
        offset = this->startX - startX;
        startX = this->startX;
    }
    if(startX > endX) {
        return false;
    }

    uint16_t width = this->endX - this->startX;
    y -= this->startY;
    memcpy(
        this->frameBuffer + this->bufferPointer + (y * width) + (startX - this->startX),
        pixels + offset,
        (endX - startX) * 2
    );
    return true;
}

bool CECanvasTFTLCD320x240::setPixel(unsigned int x, unsigned int y, uint16_t color) {
    if(x >= this->endX || y >= this->endY || x < this->startX || y < this->startY) {
        return false;
    }

    x -= this->startX;
    y -= this->startY;
    uint16_t width = this->endX - this->startX;
    this->frameBuffer[this->bufferPointer + (y * width + x)] = color;
    return true;
}

bool CECanvasTFTLCD320x240::clearPixel(unsigned int x, unsigned int y) {
    return this->setPixel(x, y, this->getBackgroundColor());
}

unsigned int CECanvasTFTLCD320x240::getCanvasWidth() {
    return LCD_WIDTH_PIXELS;
}
unsigned int CECanvasTFTLCD320x240::getCanvasHeight() {
    return LCD_HEIGHT_PIXELS;
}

bool CECanvasTFTLCD320x240::render() {
    this->lcdAPI->waitBufferTransfer();

    this->lcdAPI->sendBuffer(
        this->frameBuffer + this->bufferPointer,
        this->startX,
        this->startY,
        this->endX,
        this->endY
    );

    size_t width = this->endX - this->startX;
    size_t height = this->endY - this->startY;
    this->bufferPointer += (width * height);

    return true;
}

void CECanvasTFTLCD320x240::clear() {
    const CEColor bgColor = this->getBackgroundColor();
    const CEColor white = {0xFF, 0xFF, 0xFF};
    const CEColor black = {0x0, 0x0, 0x0};

    uint16_t width = this->endX - this->startX;
    uint16_t height = this->endY - this->startY;

    if(bgColor == white) {
        memset(this->frameBuffer + this->bufferPointer, 0xFF, width * height * 2);
        return;
    }
    if(bgColor == black) {
        memset(this->frameBuffer + this->bufferPointer, 0x0, width * height * 2);
        return;
    }

    for (size_t i = this->startX; i < this->endX; i++) {
        for (size_t j = this->startY; j < this->endY; j++) {
            this->setPixel(i, j, bgColor);
        }
    }
}

bool CECanvasTFTLCD320x240::prepareWindow(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY) {
    if(
        (endX == 0 && endY == 0) ||
        startX > endX ||
        startY > endY ||
        startX >= this->getCanvasWidth() ||
        startY >= this->getCanvasHeight()
    ) {
        return false;
    }
    if(endX > this->getCanvasWidth()) {
        endX = this->getCanvasWidth();
    }
    if(endY > this->getCanvasHeight()) {
        endY = this->getCanvasHeight();
    }

    this->startX = (uint16_t) startX;
    this->startY = (uint16_t) startY;
    this->endX = (uint16_t) endX;
    this->endY = (uint16_t) endY;

    size_t width = endX - startX;
    size_t height = endY - startY;
    if((this->bufferPointer + width * height) >= this->getCanvasWidth() * this->getCanvasHeight()) {
        this->bufferPointer = 0;
        this->lcdAPI->waitBufferTransfer();
    }

    this->clear();
    return true;
}
