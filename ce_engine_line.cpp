#include "cleytin_engine.h"

CELine::CELine(const CEPoint &start, const CEPoint &end)
{
    this->start = new CEPoint(start.x, start.y);
    this->end = new CEPoint(end.x, end.y);
}

CELine::~CELine()
{
    delete this->start;
    delete this->end;
}

int CELine::calculateSideOfPoint(CEPoint *point)
{
    return (point->y - this->start->y) * (this->end->x - this->start->x) - (point->x - this->start->x) * (this->end->y - this->start->y);
}

int CELine::getSize()
{
    return this->start->distanceTo(*this->end);
}
