#include "ce_canvas.h"

void CECanvas::setBackgroundColor(const CEColor color) {
    this->backgroundColor = color;
}

CEColor CECanvas::getBackgroundColor() {
    return this->backgroundColor;
}
