#include "cleytin_engine.h"
#include "cleytin_engine_utils.h"

CEPoint::CEPoint(int x, int y)
{
    this->x = x;
    this->y = y;
}

bool CEPoint::operator==(const CEPoint &dot)
{
    return this->x == dot.x && this->y == dot.y;
}

bool CEPoint::operator!=(const CEPoint &dot)
{
    return this->x != dot.x || this->y != dot.y;
}

void CEPoint::rotate(CEPoint *rotationCenter, uint16_t degrees)
{
    if (degrees == 0)
    {
        return;
    }

    int normX = this->x - rotationCenter->x;
    int normY = this->y - rotationCenter->y;

    this->x = (normX * cosLookUp[degrees] - normY * sinLookUp[degrees]) + rotationCenter->x;
    this->y = (normX * sinLookUp[degrees] + normY * cosLookUp[degrees]) + rotationCenter->y;
}

CEPoint *CEPoint::clone()
{
    return new CEPoint(this->x, this->y);
}

unsigned int CEPoint::distanceTo(const CEPoint point)
{
    int dX = this->x - point.x;
    int dY = this->y - point.y;

    return (unsigned int)round(sqrt(dX * dX + dY * dY));
}
