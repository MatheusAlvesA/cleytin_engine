#include "ce_color_bitmap_sprite_animation.h"

CEColorBitmapSpriteAnimation::CEColorBitmapSpriteAnimation() {
   this->sprites = new std::vector<const uint16_t *>();
   this->bitmap = NULL;
   this->bkpSprite = NULL;
   this->finished = true;
   this->duration = 0;
   this->startTime = 0;
}

CEColorBitmapSpriteAnimation::~CEColorBitmapSpriteAnimation() {
   delete this->sprites;
}

void CEColorBitmapSpriteAnimation::setFramesFrom(std::vector<const uint16_t *> *sprites) {
   this->sprites->clear();
   for (size_t i = 0; i < sprites->size(); i++) {
      this->sprites->push_back(sprites->at(i));
   }
}

void CEColorBitmapSpriteAnimation::start() {
   this->bitmap = dynamic_cast<CEColorfulBitmap *>(this->object);
   if(this->bitmap == NULL) {
      this->bkpSprite = NULL;
      printf("[ERRO] CEColorBitmapSpriteAnimation::start() - O objeto deve ser um CEColorfulBitmap\n");
      return;
   }
   this->finished = false;
   this->bitmap->setBuffer(this->sprites->at(0));
   this->startTime = esp_timer_get_time();
   this->bkpSprite = bitmap->getBuffer();
}

void CEColorBitmapSpriteAnimation::stop() {
   this->finished = true;
   if(this->bitmap == NULL) {
      return;
   }
   this->bitmap->setBuffer(this->bkpSprite);
}

void CEColorBitmapSpriteAnimation::loop() {
   if(this->finished || this->bitmap == NULL) {
      return;
   }
   uint64_t elapsed = esp_timer_get_time() - this->startTime;
   uint64_t duration = ((uint64_t)this->duration) * 1000;
   if(elapsed >= duration) {
      this->stop();
      this->bitmap->setBuffer(this->sprites->back());
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
