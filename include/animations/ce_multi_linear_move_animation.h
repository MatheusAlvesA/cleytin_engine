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
    void setSteps(std::vector<CEPoint *> *steps);
    bool isFinished();
    void setDuration(uint64_t duration);

protected:
    std::vector<CEPoint *> *steps;
    uint64_t duration;
    uint64_t startTime;
    uint64_t totalDistance;
    bool finished;
};

#endif
