#ifndef CE_MOVE_CONTROLS_H
#define CE_MOVE_CONTROLS_H

#include "ce_graphic_object.h"
#include "cleytin_engine.h"
#include "cleytin_controls.h"
#include "esp_timer.h"

class CEMoveControls {
public:
    CEMoveControls();
    bool moveUp(CEGraphicObject *object);
    bool moveUpLeft(CEGraphicObject *object);
    bool moveUpRight(CEGraphicObject *object);
    bool moveDown(CEGraphicObject *object);
    bool moveDownLeft(CEGraphicObject *object);
    bool moveDownRight(CEGraphicObject *object);
    bool moveLeft(CEGraphicObject *object);
    bool moveRight(CEGraphicObject *object);
    void stop();
    void setPixelsPerSecond(unsigned int pixelsPerSecond);
    bool handleControls(CleytinControls *controls, CEGraphicObject *object);

private :
    int64_t lastMoveTime;
    unsigned int pixelsPerSecond;

    unsigned int calculatePixelsToMove();
};

#endif
