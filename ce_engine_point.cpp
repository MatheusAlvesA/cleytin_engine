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
