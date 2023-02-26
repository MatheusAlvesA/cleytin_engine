#include "ce_font.h"

CEFont::~CEFont() {/* EMPTY */}

char *CEFont::parseString(const char *str) {
    return utf8ToLatin1(str);
}
