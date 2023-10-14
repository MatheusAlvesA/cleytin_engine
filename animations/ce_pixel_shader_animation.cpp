#include "ce_pixel_shader_animation.h"

CEPixelShaderAnimation::CEPixelShaderAnimation() {
   this->shader = NULL;
   this->finished = true;
   this->duration = 0;
   this->startTime = 0;
}

void CEPixelShaderAnimation::setShader(std::function<void(CEGraphicObject*, int, int, CEColor *)> shader) {
   this->shader = shader;
}

void CEPixelShaderAnimation::start() {
   this->finished = false;
   this->object->setPixelShaderBeforeEfects(this->shader);
   this->startTime = esp_timer_get_time();
}

void CEPixelShaderAnimation::stop() {
   this->finished = true;
   this->startTime = 0;
   this->object->setPixelShaderBeforeEfects(NULL);
}

void CEPixelShaderAnimation::loop() {
   if(this->finished || this->object == NULL || this->shader == NULL) {
      return;
   }
   uint64_t elapsed = esp_timer_get_time() - this->startTime;
   uint64_t duration = ((uint64_t)this->duration) * 1000;
   if(elapsed >= duration) {
      this->stop();
      return;
   }
}

void CEPixelShaderAnimation::setDuration(uint64_t duration) {
   this->duration = duration;
}

bool CEPixelShaderAnimation::isFinished() {
   return this->finished;
}
