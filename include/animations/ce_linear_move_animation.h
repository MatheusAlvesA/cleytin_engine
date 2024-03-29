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
    void setStartPosition(int x, int y);
    void setEndPosition(int x, int y);
    void setDuration(uint64_t duration);

protected:
    int startX;
    int startY;
    int endX;
    int endY;
    uint64_t duration;
    uint64_t startTime;
    bool finished;
};

#endif
