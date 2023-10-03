#ifndef CE_TEXT_REVEAL_ANIMATION_H
#define CE_TEXT_REVEAL_ANIMATION_H

#include "ce_animation.h"
#include "ce_text.h"

class CETextRevealAnimation : public CEAnimation {
public:
    CETextRevealAnimation();
    ~CETextRevealAnimation();
    void start();
    void stop();
    void loop();
    bool isFinished();
    void setDuration(uint64_t duration);

protected:
    void prepareTextObject();
    char *getSlice(size_t size);
    uint64_t duration;
    uint64_t startTime;
    size_t textLength;
    bool finished;
    CEText *textObject;
    char *bkpTextBuffer;
};

#endif
