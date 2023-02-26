#ifndef CE_LINEAR_MOVE_ANIMATION_H
#define CE_LINEAR_MOVE_ANIMATION_H

#include "ce_animation.h"

class CELinearMoveAnimation : public CEAnimation {
public:
    CELinearMoveAnimation();
    void start();
    void stop();
    void loop();
    bool isFinished();
    void setStartPosition(unsigned int x, unsigned int y);
    void setEndPosition(unsigned int x, unsigned int y);
    void setDuration(uint64_t duration);

protected:
    unsigned int startX;
    unsigned int startY;
    unsigned int endX;
    unsigned int endY;
    uint64_t duration;
    uint64_t startTime;
    bool finished;
};

#endif
