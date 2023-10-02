#ifndef CE_TIMER_H
#define CE_TIMER_H

#include "cleytin_commons.h"

class CETimer {
public:
    CETimer(uint64_t interval);
    bool check();
    void reset();
    uint64_t elapsed();

private:
    uint64_t interval;
    uint64_t startTime;
};

#endif
