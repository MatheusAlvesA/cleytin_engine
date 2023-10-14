#ifndef CE_PIXEL_SHADER_ANIMATION_H
#define CE_PIXEL_SHADER_ANIMATION_H

#include "ce_animation.h"
#include "ce_colorful_bitmap.h"
#include <vector>

class CEPixelShaderAnimation : public CEAnimation {
public:
    CEPixelShaderAnimation();
    void start();
    void stop();
    void loop();
    bool isFinished();
    void setDuration(uint64_t duration);
    void setShader(std::function<void(CEGraphicObject*, int, int, CEColor *)> shader);

protected:
    std::function<void(CEGraphicObject*, int, int, CEColor *)> shader;
    const uint16_t *bkpSprite;
    CEColorfulBitmap *bitmap;
    uint64_t duration;
    uint64_t startTime;
    bool finished;
};

#endif
