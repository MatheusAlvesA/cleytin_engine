#include "ce_canvas_tftlcd_320x240.h"

CECanvasTFTLCD320x240::CECanvasTFTLCD320x240() {
    this->frameBuffer = new uint16_t[this->getCanvasWidth() * this->getCanvasHeight()];;
    this->setBackgroundColor({0xFF, 0xFF, 0xFF});
    this->prepareWindow(0, 0, this->getCanvasWidth(), this->getCanvasHeight());

    this->lcdAPI = new CleytinTFTAPI();
    this->render();
}

CECanvasTFTLCD320x240::~CECanvasTFTLCD320x240() {
    delete this->frameBuffer;
    delete this->lcdAPI;
}

bool CECanvasTFTLCD320x240::setPixel(unsigned int x, unsigned int y, const CEColor color) {
    if(x >= this->endX || y >= this->endY || x < this->startX || y < this->startY) {
        return false;
    }

    x -= this->startX;
    y -= this->startY;
    uint16_t width = this->endX - this->startX;
    this->frameBuffer[y * width + x] = this->color2rgb565(color);
    return true;
}

bool CECanvasTFTLCD320x240::clearPixel(unsigned int x, unsigned int y) {
    return this->setPixel(x, y, this->getBackgroundColor());
}

uint16_t CECanvasTFTLCD320x240::color2rgb565(const CEColor color) {
    uint8_t high = 0;
    uint8_t low  = 0;

    high = color.red & 0b11111000;                   // Primeiros 5 bits são o vermelho
    high |= color.green >> 5;                        // Bits 6 até 8 são os primeiros 3 bits do verde
    low  = (color.green & 0b11111100) << 3;          // Bits 9 até 11 sãos os últimos 3 bits do verde
    low  |= color.blue  >> 3;                        // Bits 12 até 16 são o azul

    // É necessário inverter os bytes pois a tela funciona em big endian
    return (uint16_t) ((((uint16_t) low) << 8) | high);
}

unsigned int CECanvasTFTLCD320x240::getCanvasWidth() {
    return LCD_WIDTH_PIXELS;
}
unsigned int CECanvasTFTLCD320x240::getCanvasHeight() {
    return LCD_HEIGHT_PIXELS;
}

bool CECanvasTFTLCD320x240::render() {
    this->lcdAPI->sendBuffer(
        this->frameBuffer,
        this->startX,
        this->startY,
        this->endX,
        this->endY
    );
    this->lcdAPI->waitBufferTransfer();

    return true;
}

void CECanvasTFTLCD320x240::clear() {
    const CEColor bgColor = this->getBackgroundColor();
    const CEColor white = {0xFF, 0xFF, 0xFF};
    const CEColor black = {0x0, 0x0, 0x0};

    uint16_t width = this->endX - this->startX;
    uint16_t height = this->endY - this->startY;

    if(bgColor == white) {
        memset(this->frameBuffer, 0xFF, width * height * 2);
        return;
    }
    if(bgColor == black) {
        memset(this->frameBuffer, 0x0, width * height * 2);
        return;
    }

    for (size_t i = this->startX; i < this->endX; i++) {
        for (size_t j = this->startY; j < this->endY; j++) {
            this->setPixel(i, j, bgColor);
        }
    }
}

void CECanvasTFTLCD320x240::prepareWindow(unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY) {
    if(endX == 0 || endY == 0 || startX > endX || startY > endY) {
        return;
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

    this->clear();
}
