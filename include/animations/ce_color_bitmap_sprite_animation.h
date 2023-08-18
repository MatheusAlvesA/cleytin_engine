#ifndef CE_LINEAR_COLOR_BITMAP_SPRITE_ANIMATION_H
#define CE_LINEAR_COLOR_BITMAP_SPRITE_ANIMATION_H

#include "ce_animation.h"
#include "ce_colorful_bitmap.h"
#include <vector>

class CEColorBitmapSpriteAnimation : public CEAnimation {
public:
    CEColorBitmapSpriteAnimation();
    ~CEColorBitmapSpriteAnimation();
    void start();
    void stop();
    void loop();
    bool isFinished();
    void setDuration(uint64_t duration);
    void setBitmapObject(CEColorfulBitmap *bitmap);
    void setFramesFrom(std::vector<const uint16_t *> *sprites);

protected:
    std::vector<const uint16_t *> *sprites;
    const uint16_t *bkpSprite;
    CEColorfulBitmap *bitmap;
    uint64_t duration;
    uint64_t startTime;
    bool finished;
};

#endif
