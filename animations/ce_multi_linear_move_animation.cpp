#include "ce_multi_linear_move_animation.h"

CEMultiLinearMoveAnimation::CEMultiLinearMoveAnimation()
{
   this->steps = new std::vector<CEPoint *>();
   this->finished = true;
   this->duration = 0;
   this->startTime = 0;
   this->totalDistance = 0;
   this->isLooping = false;
}

void CEMultiLinearMoveAnimation::setIsLooping(bool isLooping) {
   this->isLooping = isLooping;
}

void CEMultiLinearMoveAnimation::setSteps(std::vector<CEPoint *> *steps)
{
   for (size_t i = 0; i < this->steps->size(); i++)
   {
      delete this->steps->at(i);
   }
   this->steps->clear();
   this->totalDistance = 0;
   for (size_t i = 0; i < steps->size(); i++)
   {
      this->steps->push_back(steps->at(i)->clone());
      if(i > 0) {
         this->totalDistance += this->steps->at(i)->distanceTo(*this->steps->at(i - 1));
      }
   }
}

CEMultiLinearMoveAnimation::~CEMultiLinearMoveAnimation()
{
   delete_pointers_vector<CEPoint>(this->steps);
}

void CEMultiLinearMoveAnimation::start()
{
   this->finished = false;
   if(this->steps->size() > 0) {
      this->object->setPos(this->steps->at(0)->x, this->steps->at(0)->y);
   }
   this->startTime = esp_timer_get_time();
}

void CEMultiLinearMoveAnimation::stop()
{
   this->finished = true;
   this->startTime = 0;
}

void CEMultiLinearMoveAnimation::loop()
{
   if (this->finished || this->steps->size() <= 1 || this->duration == 0)
   {
      return;
   }
   uint64_t elapsed = esp_timer_get_time() - this->startTime;
   uint64_t duration = ((uint64_t)this->duration) * 1000;
   float percentage = ((float)elapsed) / duration;
   uint64_t currentDistance = ((float) this->totalDistance) * percentage;
   size_t step = 1;
   uint64_t accDistance = this->steps->at(0)->distanceTo(*this->steps->at(1));
   for (; step < this->steps->size() - 1 && accDistance < currentDistance; step++)
   {
      accDistance += this->steps->at(step)->distanceTo(*this->steps->at(step+1));
   }
   step--;

   if (elapsed >= duration || step >= this->steps->size() - 1)
   {
      CEPoint *lastStep = this->steps->at(this->steps->size() - 1);
      this->object->setPos(lastStep->x, lastStep->y);
      if(this->isLooping) {
         this->start();
      } else {
         this->stop();
      }
      return;
   }

   uint64_t distanceUntilNext = accDistance - currentDistance;
   CEPoint *currentStep = this->steps->at(step);
   CEPoint *nextStep = this->steps->at(step + 1);
   unsigned int distance = currentStep->distanceTo(*nextStep);
   percentage = ((float) (distance - distanceUntilNext)) / distance; 

   int x = currentStep->x;
   int y = currentStep->y;

   float deltaX = (float) (nextStep->x - currentStep->x);
   x += deltaX * percentage;

   float deltaY = (float) (nextStep->y - currentStep->y);
   y += deltaY * percentage;

   this->object->setPos(x, y);
}

void CEMultiLinearMoveAnimation::setDuration(uint64_t duration)
{
   this->duration = duration;
}

bool CEMultiLinearMoveAnimation::isFinished()
{
   return this->finished;
}
