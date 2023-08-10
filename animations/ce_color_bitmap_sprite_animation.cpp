#include "ce_color_bitmap_sprite_animation.h"

CEColorBitmapSpriteAnimation::CEColorBitmapSpriteAnimation() {
   this->sprites = new std::vector<uint16_t *>();
   this->bitmap = NULL;
   this->bkpSprite = NULL;
   this->finished = true;
   this->duration = 0;
   this->startTime = 0;
}

CEColorBitmapSpriteAnimation::~CEColorBitmapSpriteAnimation() {
   delete this->sprites;
}

void CEColorBitmapSpriteAnimation::setBitmapObject(CEColorfulBitmap *bitmap) {
   this->bitmap = bitmap;
   this->bkpSprite = bitmap->getBuffer();
}

void CEColorBitmapSpriteAnimation::start() {
   this->finished = false;
   this->bitmap->setBuffer(this->sprites->at(0));
   this->startTime = esp_timer_get_time();
}

void CEColorBitmapSpriteAnimation::stop() {
   this->finished = true;
   this->bitmap->setBuffer(this->bkpSprite);
}

void CEColorBitmapSpriteAnimation::loop() {
   if(this->finished) {
      return;
   }
   uint64_t elapsed = esp_timer_get_time() - this->startTime;
   uint64_t duration = ((uint64_t)this->duration) * 1000;
   if(elapsed >= duration) {
      this->stop();
      return;
   }
   float percentage = (float) ((float)elapsed / duration);
   size_t index = (size_t) (percentage * this->sprites->size());

   this->bitmap->setBuffer(this->sprites->at(index));
}

void CEColorBitmapSpriteAnimation::setDuration(uint64_t duration) {
   this->duration = duration;
}

bool CEColorBitmapSpriteAnimation::isFinished() {
   return this->finished;
}
