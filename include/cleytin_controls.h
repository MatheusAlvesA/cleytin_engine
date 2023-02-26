#ifndef CLEYTIN_CONTROLS_H
#define CLEYTIN_CONTROLS_H

#include <vector>
#include "esp_system.h"
#include "cleytin_commons.h"

#define CLEYTIN_BTN_UP GPIO_NUM_34
#define CLEYTIN_BTN_RIGHT GPIO_NUM_22
#define CLEYTIN_BTN_DOWN GPIO_NUM_23
#define CLEYTIN_BTN_LEFT GPIO_NUM_25

#define CLEYTIN_BTN_START GPIO_NUM_35
#define CLEYTIN_BTN_A GPIO_NUM_26
#define CLEYTIN_BTN_B GPIO_NUM_27
#define CLEYTIN_BTN_C GPIO_NUM_32


class CleytinControls {
public:
    static void init();
    bool getUp(bool debouncing = true);
    bool getRight(bool debouncing = true);
    bool getDown(bool debouncing = true);
    bool getLeft(bool debouncing = true);
    bool getStart(bool debouncing = true);
    bool getA(bool debouncing = true);
    bool getB(bool debouncing = true);
    bool getC(bool debouncing = true);
    gpio_num_t waitClick();

private:
    bool readBtnState(gpio_num_t btn, bool debouncing);
};

#endif
