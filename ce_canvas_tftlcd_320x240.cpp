#include "ce_canvas_tftlcd_320x240.h"

CECanvasTFTLCD320x240::CECanvasTFTLCD320x240() {
    this->frameBuffer = new uint16_t[LCD_WIDTH_PIXELS * LCD_HEIGHT_PIXELS];
    memset(this->frameBuffer, 0xFF, LCD_WIDTH_PIXELS * LCD_HEIGHT_PIXELS * 2);

    this->lcdAPI = new CleytinTFTAPI();
    this->startRender();
    this->waitRenderFinish();

    this->setBackgroundColor({0xFF, 0xFF, 0xFF});
}

CECanvasTFTLCD320x240::~CECanvasTFTLCD320x240() {
    delete this->frameBuffer;
    delete this->lcdAPI;
}

bool CECanvasTFTLCD320x240::setPixel(unsigned int x, unsigned int y, const CEColor color) {
    if(x >= LCD_WIDTH_PIXELS || y >= LCD_HEIGHT_PIXELS) {
        return false;
    }
    this->frameBuffer[y * LCD_WIDTH_PIXELS + x] = this->color2rgb565(color);
    return true;
}
bool CECanvasTFTLCD320x240::clearPixel(unsigned int x, unsigned int y) {
    if(x >= LCD_WIDTH_PIXELS || y >= LCD_HEIGHT_PIXELS) {
        return false;
    }
    this->frameBuffer[y * LCD_WIDTH_PIXELS + x] = this->color2rgb565(this->getBackgroundColor());
    return true;
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

void CECanvasTFTLCD320x240::startRender() {
    this->lcdAPI->sendBuffer(this->frameBuffer);
}

void CECanvasTFTLCD320x240::waitRenderFinish() {
    this->lcdAPI->waitBufferTransfer();
}

void CECanvasTFTLCD320x240::clear() {
    const CEColor bgColor = this->getBackgroundColor();
    const CEColor white = {0xFF, 0xFF, 0xFF};
    const CEColor black = {0x0, 0x0, 0x0};
    if(bgColor == white) {
        memset(this->frameBuffer, 0xFF, LCD_WIDTH_PIXELS * LCD_HEIGHT_PIXELS * 2);
        return;
    }
    if(bgColor == black) {
        memset(this->frameBuffer, 0x0, LCD_WIDTH_PIXELS * LCD_HEIGHT_PIXELS * 2);
        return;
    }

    for (size_t i = 0; i < this->getCanvasWidth(); i++) {
        for (size_t j = 0; j < this->getCanvasHeight(); j++) {
            this->setPixel(i, j, bgColor);
        }
    }
}