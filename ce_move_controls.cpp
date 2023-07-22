#include "ce_move_controls.h"

CEMoveControls::CEMoveControls() {
    this->lastMoveTime = 0;
    this->pixelsPerSecond = 10;
}

void CEMoveControls::setPixelsPerSecond(unsigned int pixelsPerSecond) {
    this->pixelsPerSecond = pixelsPerSecond;
}

bool CEMoveControls::moveUp(CEGraphicObject *object) {
    unsigned int pixelsToMove = this->calculatePixelsToMove();
    if(pixelsToMove > object->getPosY()) { // Tentando mover para fora da tela
        this->stop();
        return false;
    }
    if(pixelsToMove == 0) {
        return false;
    }
    object->setPosY(object->getPosY() - pixelsToMove);
    this->lastMoveTime = esp_timer_get_time();
    return true;
}

bool CEMoveControls::moveUpLeft(CEGraphicObject *object) {
    unsigned int pixelsToMove = this->calculatePixelsToMove();
    if(
        pixelsToMove > object->getPosY() ||
        pixelsToMove > object->getPosX()
    ) { // Tentando mover para fora da tela
        this->stop();
        return false;
    }
    if(pixelsToMove == 0) {
        return false;
    }
    object->setPos(object->getPosX() - pixelsToMove, object->getPosY() - pixelsToMove);
    this->lastMoveTime = esp_timer_get_time();
    return true;
}

bool CEMoveControls::moveDownLeft(CEGraphicObject *object) {
    unsigned int pixelsToMove = this->calculatePixelsToMove();
    unsigned int maxY = object->getMaxY() - object->getRenderWindowHeight();
    int newPosY = object->getPosY() + (int) pixelsToMove;

    if(
        pixelsToMove > object->getPosY() ||
        newPosY > maxY
    ) { // Tentando mover para fora da tela
        this->stop();
        return false;
    }
    if(pixelsToMove == 0) {
        return false;
    }
    object->setPos(object->getPosX() - (int) pixelsToMove, newPosY);
    this->lastMoveTime = esp_timer_get_time();
    return true;
}

bool CEMoveControls::moveDownRight(CEGraphicObject *object) {
    unsigned int pixelsToMove = this->calculatePixelsToMove();
    unsigned int maxY = object->getMaxY() - object->getRenderWindowHeight();
    int newPosY = object->getPosY() + pixelsToMove;
    unsigned int maxX = object->getMaxX() - object->getRenderWindowWidth();
    int newPosX = object->getPosX() + pixelsToMove;

    if(
        newPosX > maxX ||
        newPosY > maxY
    ) { // Tentando mover para fora da tela
        this->stop();
        return false;
    }
    if(pixelsToMove == 0) {
        return false;
    }
    object->setPos(newPosX, newPosY);
    this->lastMoveTime = esp_timer_get_time();
    return true;
}

bool CEMoveControls::moveUpRight(CEGraphicObject *object) {
    unsigned int pixelsToMove = this->calculatePixelsToMove();
    unsigned int maxX = object->getMaxX() - object->getRenderWindowWidth();
    int newPosX = object->getPosX() + pixelsToMove;
    if(
        pixelsToMove > object->getPosY() ||
        newPosX > maxX
    ) { // Tentando mover para fora da tela
        this->stop();
        return false;
    }
    if(pixelsToMove == 0) {
        return false;
    }
    object->setPos(newPosX, object->getPosY() - pixelsToMove);
    this->lastMoveTime = esp_timer_get_time();
    return true;
}

bool CEMoveControls::moveDown(CEGraphicObject *object) {
    unsigned int pixelsToMove = this->calculatePixelsToMove();
    unsigned int maxY = object->getMaxY() - object->getRenderWindowHeight();
    int newPosY = object->getPosY() + pixelsToMove;

    if(newPosY > maxY) { // Tentando mover para fora da tela
        this->stop();
        return false;
    }
    if(pixelsToMove == 0) {
        return false;
    }
    object->setPosY(newPosY);
    this->lastMoveTime = esp_timer_get_time();
    return true;
}

bool CEMoveControls::moveLeft(CEGraphicObject *object) {
    unsigned int pixelsToMove = this->calculatePixelsToMove();
    if(pixelsToMove > object->getPosX()) { // Tentando mover para fora da tela
        this->stop();
        return false;
    }
    if(pixelsToMove == 0) {
        return false;
    }
    object->setPosX(object->getPosX() - (int) pixelsToMove);
    this->lastMoveTime = esp_timer_get_time();
    return true;
}

bool CEMoveControls::moveRight(CEGraphicObject *object) {
    unsigned int pixelsToMove = this->calculatePixelsToMove();
    unsigned int maxX = object->getMaxX() - object->getRenderWindowWidth();
    int newPosX = object->getPosX() + pixelsToMove;

    if(newPosX > maxX) { // Tentando mover para fora da tela
        this->stop();
        return false;
    }
    if(pixelsToMove == 0) {
        return false;
    }
    object->setPosX(newPosX);
    this->lastMoveTime = esp_timer_get_time();
    return true;
}

unsigned int CEMoveControls::calculatePixelsToMove() {
    if(this->lastMoveTime == 0) {
        this->lastMoveTime = esp_timer_get_time();
    }
    uint64_t elapsedMicroseconds = esp_timer_get_time() - this->lastMoveTime;
    uint64_t microsecondsPerPixel = (1000 * 1000) / this->pixelsPerSecond; // Quantos microsegundos para mover um pixel
    uint64_t pixelsToMove = elapsedMicroseconds / microsecondsPerPixel;

    return (unsigned int) pixelsToMove;
}

void CEMoveControls::stop() {
    this->lastMoveTime = 0;
}

bool CEMoveControls::handleControls(CleytinControls *controls, CEGraphicObject *object) {
    if(controls->getUp() && controls->getLeft()) {
        return this->moveUpLeft(object);
    }
    if(controls->getUp() && controls->getRight()) {
        return this->moveUpRight(object);
    }
    if(controls->getDown() && controls->getLeft()) {
        return this->moveDownLeft(object);
    }
    if(controls->getDown() && controls->getRight()) {
        return this->moveDownRight(object);
    }
    if(controls->getUp()) {
        return this->moveUp(object);
    }
    if(controls->getDown()) {
        return this->moveDown(object);
    }
    if(controls->getLeft()) {
        return this->moveLeft(object);
    }
    if(controls->getRight()) {
        return this->moveRight(object);
    }
    this->stop();
    return false;
}
