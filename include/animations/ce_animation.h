#ifndef CE_ANIMATION_H
#define CE_ANIMATION_H

#include "cleytin_engine.h"

class CEAnimation {
public:
    CEAnimation();
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void loop() = 0;
    virtual bool isFinished() = 0;
    void setObject(CEGraphicObject *object);

protected:
    CEGraphicObject *object;
};

#endif
