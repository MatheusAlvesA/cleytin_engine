#include "ce_linear_move_animation.h"

CELinearMoveAnimation::CELinearMoveAnimation() {
   this->startX = 0;
   this->startY = 0;
   this->endX = 0;
   this->endY = 0;
   this->finished = true;
   this->duration = 0;
   this->startTime = 0;
}

void CELinearMoveAnimation::start() {
   this->finished = false;
   this->object->setPos(this->startX, this->startY);
   this->startTime = esp_timer_get_time();
}

void CELinearMoveAnimation::stop() {
   this->finished = true;
}

void CELinearMoveAnimation::loop() {
   if(this->finished) {
      return;
   }
   unsigned int elapsed = this->startTime - esp_timer_get_time();
   unsigned int x = this->endX;
   unsigned int y = this->endY;
   if(elapsed >= this->duration) {
      this->object->setPos(x, y);
      this->finished = true;
   }
   float percentage = (float) ((float)elapsed / startTime);

   int deltaX = (int) this->endX - (int) this->startX;
   deltaX *= percentage;
   x += deltaX;

   int deltaY = (int) this->endY - (int) this->startY;
   deltaY *= percentage;
   y += deltaY;

   this->object->setPos(x, y);
}

void CELinearMoveAnimation::setDuration(uint64_t duration) {
   this->duration = duration;
}

bool CELinearMoveAnimation::isFinished() {
   return this->finished;
}

void CELinearMoveAnimation::setStartPosition(unsigned int x, unsigned int y) {
   this->startX = x;
   this->startY = y;
}

void CELinearMoveAnimation::setEndPosition(unsigned int x, unsigned int y) {
   this->endX = x;
   this->endY = y;
}
