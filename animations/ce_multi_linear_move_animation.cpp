#include "ce_multi_linear_move_animation.h"

CEMultiLinearMoveAnimation::CEMultiLinearMoveAnimation()
{
   this->steps = new std::vector<CELine *>();
   this->finished = true;
   this->duration = 0;
   this->startTime = 0;
}

void CEMultiLinearMoveAnimation::setSteps(std::vector<CELine *> *steps)
{
   for (size_t i = 0; i < this->steps->size(); i++)
   {
      delete this->steps->at(i);
   }
   this->steps->clear();
   for (size_t i = 0; i < steps->size(); i++)
   {
      this->steps->push_back(steps->at(i)->clone());
   }
}

CEMultiLinearMoveAnimation::~CEMultiLinearMoveAnimation()
{
   delete_pointers_vector<CELine>(this->steps);
}

void CEMultiLinearMoveAnimation::start()
{
   this->finished = false;
   this->object->setPos(0, 0);
   this->startTime = esp_timer_get_time();
}

void CEMultiLinearMoveAnimation::stop()
{
   this->finished = true;
   this->startTime = 0;
}

void CEMultiLinearMoveAnimation::loop()
{
   if (this->finished || this->steps->size() <= 0)
   {
      return;
   }
   uint64_t elapsed = esp_timer_get_time() - this->startTime;
   uint64_t duration = ((uint64_t)this->duration) * 1000;
   if (elapsed >= duration)
   {
      CELine *lastStep = this->steps->at(this->steps->size() - 1);
      this->object->setPos(lastStep->end->x, lastStep->end->y);
      this->stop();
      return;
   }
   uint64_t stepDuration = duration / this->steps->size();
   uint64_t step = elapsed / stepDuration;
   uint64_t stepElapsed = elapsed - (step * stepDuration);
   CELine *currentStep = this->steps->at(step);

   int x = currentStep->start->x;
   int y = currentStep->start->y;

   int deltaX = currentStep->end->x - currentStep->start->x;
   deltaX = (deltaX * stepElapsed) / (deltaX * stepDuration);
   x += deltaX;

   int deltaY = currentStep->end->y - currentStep->start->y;
   deltaY = (deltaY * stepElapsed) / (deltaY * stepDuration);
   y += deltaY;

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
