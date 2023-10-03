#include "ce_text_reveal_animation.h"

CETextRevealAnimation::CETextRevealAnimation() {
   this->finished = true;
   this->duration = 0;
   this->startTime = 0;
   this->textObject = NULL;
   this->bkpTextBuffer = NULL;
   this->textLength = 0;
}

CETextRevealAnimation::~CETextRevealAnimation() {
   if(this->bkpTextBuffer != NULL) {
      delete this->bkpTextBuffer;
   }
}

void CETextRevealAnimation::start() {
   this->finished = false;
   this->startTime = esp_timer_get_time();
   this->prepareTextObject();
}

void CETextRevealAnimation::prepareTextObject() {
   if(this->bkpTextBuffer != NULL) {
      delete this->bkpTextBuffer;
      this->bkpTextBuffer = NULL;
      this->textLength = 0;
   }

   this->textObject = dynamic_cast<CEText *>(this->object);
   if(this->textObject == NULL) {
      return;
   }
   this->textLength = strlen(this->textObject->getText());
   this->bkpTextBuffer = new char[this->textLength + 1];
   strcpy(this->bkpTextBuffer, this->textObject->getText());
   printf("Text: %s\n", this->bkpTextBuffer);
}

char *CETextRevealAnimation::getSlice(size_t size) {
   if(this->bkpTextBuffer == NULL || this->textLength == 0) {
      return NULL;
   }
   if(size > this->textLength) {
      size = this->textLength;
   }
   char *slice = new char[this->textLength + 1];
   strcpy(slice, this->bkpTextBuffer);
   for (size_t i = size; i < this->textLength; i++) {
      slice[i] = ' ';
   }
   
   return slice;
}

void CETextRevealAnimation::stop() {
   if(this->bkpTextBuffer != NULL) {
      this->textObject->setText(this->bkpTextBuffer, false);
      delete this->bkpTextBuffer;
      this->bkpTextBuffer = NULL;
      this->textLength = 0;
   }

   this->finished = true;
   this->textObject = NULL;
   this->startTime = 0;
}

void CETextRevealAnimation::loop() {
   if(this->finished || this->textObject == NULL) {
      return;
   }
   uint64_t elapsed = esp_timer_get_time() - this->startTime;
   uint64_t duration = this->duration * 1000;
   if(elapsed >= duration) {
      this->stop();
      return;
   }
   uint64_t timePerChar = duration / this->textLength;
   size_t charsToShow = elapsed / timePerChar;
   char *slice = this->getSlice(charsToShow);
   if(slice == NULL) {
      return;
   }

   this->textObject->setText(slice, false);
   delete slice;
}

void CETextRevealAnimation::setDuration(uint64_t duration) {
   this->duration = duration;
}

bool CETextRevealAnimation::isFinished() {
   return this->finished;
}

