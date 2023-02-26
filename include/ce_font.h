#ifndef CE_FONT_H
#define CE_FONT_H

#include "cleytin_commons.h"

class CEFont {
public:
    virtual ~CEFont();
    virtual size_t getCharWidth();
    virtual size_t getCharHeight();
    virtual uint8_t *getRawPonter();
    virtual char *parseString(const char *str);
    virtual unsigned int getPositionOf(unsigned char c);
};

#endif
