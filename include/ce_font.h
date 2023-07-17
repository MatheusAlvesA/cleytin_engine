#ifndef CE_FONT_H
#define CE_FONT_H

#include "cleytin_commons.h"

class CEFont {
public:
    virtual ~CEFont();
    virtual size_t getCharWidth() = 0;
    virtual size_t getCharHeight() = 0;
    virtual uint8_t *getRawPonter() = 0;
    virtual char *parseString(const char *str);
    virtual unsigned int getPositionOf(unsigned char c) = 0;
};

#endif
