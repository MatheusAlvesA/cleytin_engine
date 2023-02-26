#include "ce_8x12_font.h"

CE8x12Font::CE8x12Font() {/* EMPTY */}
CE8x12Font::~CE8x12Font() {/* EMPTY */}

size_t CE8x12Font::getCharWidth() {
    return 8;
}

size_t CE8x12Font::getCharHeight() {
    return CHAR_HEIGHT;
}

uint8_t *CE8x12Font::getRawPonter() {
    return (uint8_t *)buff;
}

unsigned int CE8x12Font::getPositionOf(unsigned char c) {
    return map_8x12_font[c % 256];
}
