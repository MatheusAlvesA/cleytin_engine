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
   int64_t elapsed = esp_timer_get_time() - this->startTime;
   int64_t duration = ((uint64_t)this->duration) * 1000;
   if(elapsed >= duration) {
      this->object->setPos(this->endX, this->endY);
      this->finished = true;
      return;
   }

   int distanceX = this->endX - this->startX;
   int distanceY = this->endY - this->startY;

   int x = this->startX;
   int y = this->startY;

   if(distanceX != 0) {
      int64_t timePerPixelX = duration / distanceX;
      int64_t currentX = elapsed / timePerPixelX;
      x += currentX;
   }

   if(distanceY != 0) {
      int64_t timePerPixelY = duration / distanceY;
      int64_t currentY = elapsed / timePerPixelY;
      y += currentY;
   }

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
