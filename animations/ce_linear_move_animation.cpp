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
   uint64_t elapsed = esp_timer_get_time() - this->startTime;
   uint64_t duration = ((uint64_t)this->duration) * 1000;
   int x = this->startX;
   int y = this->startY;
   if(elapsed >= duration) {
      this->object->setPos(this->endX, this->endY);
      this->finished = true;
   }
   float percentage = (float) ((float)elapsed / duration);

   float deltaX = (float) this->endX - (float) this->startX;
   deltaX *= percentage;
   x += (int) deltaX;

   float deltaY = (float) this->endY - (float) this->startY;
   deltaY *= percentage;
   y += (int) deltaY;

   this->object->setPos(x, y);
}

void CELinearMoveAnimation::setDuration(uint64_t duration) {
   this->duration = duration;
}

bool CELinearMoveAnimation::isFinished() {
   return this->finished;
}

void CELinearMoveAnimation::setStartPosition(int x, int y) {
   this->startX = x;
   this->startY = y;
}

void CELinearMoveAnimation::setEndPosition(int x, int y) {
   this->endX = x;
   this->endY = y;
}
