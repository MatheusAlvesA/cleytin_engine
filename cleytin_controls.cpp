#include "cleytin_controls.h"

void CleytinControls::init() {
    cleytin_set_gpio_pin(CLEYTIN_BTN_UP, CLEYTIN_GPIO_MODE_INPUT_PULLUP, GPIO_INTR_DISABLE);
    cleytin_set_gpio_pin(CLEYTIN_BTN_RIGHT, CLEYTIN_GPIO_MODE_INPUT_PULLUP, GPIO_INTR_DISABLE);
    cleytin_set_gpio_pin(CLEYTIN_BTN_DOWN, CLEYTIN_GPIO_MODE_INPUT_PULLUP, GPIO_INTR_DISABLE);
    cleytin_set_gpio_pin(CLEYTIN_BTN_LEFT, CLEYTIN_GPIO_MODE_INPUT_PULLUP, GPIO_INTR_DISABLE);

    cleytin_set_gpio_pin(CLEYTIN_BTN_START, CLEYTIN_GPIO_MODE_INPUT_PULLUP, GPIO_INTR_DISABLE);
    cleytin_set_gpio_pin(CLEYTIN_BTN_A, CLEYTIN_GPIO_MODE_INPUT_PULLUP, GPIO_INTR_DISABLE);
    cleytin_set_gpio_pin(CLEYTIN_BTN_B, CLEYTIN_GPIO_MODE_INPUT_PULLUP, GPIO_INTR_DISABLE);
    cleytin_set_gpio_pin(CLEYTIN_BTN_C, CLEYTIN_GPIO_MODE_INPUT_PULLUP, GPIO_INTR_DISABLE);
}

bool CleytinControls::readBtnState(gpio_num_t btn, bool debouncing) {
    if(!debouncing) {
        return gpio_get_level(btn) == 0;
    }
    bool r = gpio_get_level(btn) == 0;
    cleytin_delay(1);
    bool r2 = gpio_get_level(btn) == 0;
    return r && r2;
}

bool CleytinControls::getUp(bool debouncing) {
    return this->readBtnState(CLEYTIN_BTN_UP, debouncing);
}

bool CleytinControls::getRight(bool debouncing) {
    return this->readBtnState(CLEYTIN_BTN_RIGHT, debouncing);
}

bool CleytinControls::getDown(bool debouncing) {
    return this->readBtnState(CLEYTIN_BTN_DOWN, debouncing);
}

bool CleytinControls::getLeft(bool debouncing) {
    return this->readBtnState(CLEYTIN_BTN_LEFT, debouncing);
}

bool CleytinControls::getStart(bool debouncing) {
    return this->readBtnState(CLEYTIN_BTN_START, debouncing);
}

bool CleytinControls::getA(bool debouncing) {
    return this->readBtnState(CLEYTIN_BTN_A, debouncing);
}

bool CleytinControls::getB(bool debouncing) {
    return this->readBtnState(CLEYTIN_BTN_B, debouncing);
}

bool CleytinControls::getC(bool debouncing) {
    return this->readBtnState(CLEYTIN_BTN_C, debouncing);
}

gpio_num_t CleytinControls::waitClick() {
    std::vector<gpio_num_t> list;
    std::vector<gpio_num_t> notReady;
    if(!this->getA())     list.push_back(CLEYTIN_BTN_A);
    else                  notReady.push_back(CLEYTIN_BTN_A);
    if(!this->getB())     list.push_back(CLEYTIN_BTN_B);
    else                  notReady.push_back(CLEYTIN_BTN_B);
    if(!this->getC())     list.push_back(CLEYTIN_BTN_C);
    else                  notReady.push_back(CLEYTIN_BTN_C);
    if(!this->getStart()) list.push_back(CLEYTIN_BTN_START);
    else                  notReady.push_back(CLEYTIN_BTN_START);
    if(!this->getUp())    list.push_back(CLEYTIN_BTN_UP);
    else                  notReady.push_back(CLEYTIN_BTN_UP);
    if(!this->getLeft())  list.push_back(CLEYTIN_BTN_LEFT);
    else                  notReady.push_back(CLEYTIN_BTN_LEFT);
    if(!this->getDown())  list.push_back(CLEYTIN_BTN_DOWN);
    else                  notReady.push_back(CLEYTIN_BTN_DOWN);
    if(!this->getRight()) list.push_back(CLEYTIN_BTN_RIGHT);
    else                  notReady.push_back(CLEYTIN_BTN_RIGHT);

    gpio_num_t r = GPIO_NUM_NC;
    while(r < 0) {
        for (size_t i = 0; i < notReady.size(); i++) {
            if(notReady[i] > 0 && !this->readBtnState(notReady[i], true)) {
                list.push_back(notReady[i]);
                notReady[i] = GPIO_NUM_NC;
            }
        }
        for (size_t i = 0; i < list.size(); i++) {
            if(this->readBtnState(list[i], true)) {
                r = list[i];
            }
        }
        cleytin_delay(10);
    }

    while(!this->readBtnState(r, true)) cleytin_delay(10);

    return r;
}
