#include "ce_timer.h"

CETimer::CETimer(uint64_t interval) {
    this->interval = interval;
    this->startTime = esp_timer_get_time();
}

bool CETimer::check() {
    uint64_t now = esp_timer_get_time();
    if(now - this->startTime > (this->interval * 1000)) {
        return true;
    }
    return false;
}

void CETimer::reset() {
    this->startTime = esp_timer_get_time();
}

uint64_t CETimer::elapsed() {
    return (esp_timer_get_time() - this->startTime) / 1000;
}
