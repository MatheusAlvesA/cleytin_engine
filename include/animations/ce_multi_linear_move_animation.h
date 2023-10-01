#ifndef CE_MULTI_LINEAR_MOVE_ANIMATION_H
#define CE_MULTI_LINEAR_MOVE_ANIMATION_H

#include "ce_animation.h"
#include "cleytin_engine_utils.h"

class CEMultiLinearMoveAnimation : public CEAnimation
{
public:
    CEMultiLinearMoveAnimation();
    ~CEMultiLinearMoveAnimation();
    void start();
    void stop();
    void loop();
    void setSteps(std::vector<CELine *> *steps);
    bool isFinished();
    void setDuration(uint64_t duration);

protected:
    std::vector<CELine *> *steps;
    uint64_t duration;
    uint64_t startTime;
    bool finished;
};

#endif
