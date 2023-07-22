#include "ce_animation.h"

CEAnimation::CEAnimation() {
   this->object = NULL;
}

CEAnimation::~CEAnimation() {/* EMPTY */}

void CEAnimation::setObject(CEGraphicObject *object) {
   this->object = object;
}
