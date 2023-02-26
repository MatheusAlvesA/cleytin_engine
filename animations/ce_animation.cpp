#include "ce_animation.h"

CEAnimation::CEAnimation() {
   this->object = NULL;
}

void CEAnimation::setObject(CEGraphicObject *object) {
   this->object = object;
}
