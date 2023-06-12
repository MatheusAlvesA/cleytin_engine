#include "ce_canvas.h"

void CECanvas::setBackgroundColor(const CEColor color) {
    this->backgroundColor = color;
}

CEColor CECanvas::getBackgroundColor() {
    return this->backgroundColor;
}

CEColor RGB565ToColor(const uint16_t raw)
{
    CEColor r = {0, 0, 0};

    // Invertendo bytes
    uint8_t high = (uint8_t)(raw >> 8);
    uint8_t low = (uint8_t)(raw & 0xFF);
    uint16_t inverted = (uint16_t)((low << 8) | high);

    r.red = (uint8_t)((inverted >> 11) << 3);  // Primeiros 5 bits são o vermelho
    r.green = (uint8_t)((inverted >> 5) << 2); // Bits 6 até 11 são o verde
    r.blue = (uint8_t)(inverted << 3);         // Bits 12 até 16 são o azul

    return r;
}

uint16_t colorToRGB565(const CEColor color) {
    uint8_t high = 0;
    uint8_t low  = 0;

    high = color.red & 0b11111000;                   // Primeiros 5 bits são o vermelho
    high |= color.green >> 5;                        // Bits 6 até 8 são os primeiros 3 bits do verde
    low  = (color.green & 0b11111100) << 3;          // Bits 9 até 11 sãos os últimos 3 bits do verde
    low  |= color.blue  >> 3;                        // Bits 12 até 16 são o azul

    // É necessário inverter os bytes pois a tela funciona em big endian
    return (uint16_t) ((((uint16_t) low) << 8) | high);
}
