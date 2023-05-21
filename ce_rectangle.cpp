#include "ce_rectangle.h"

CERectangle::CERectangle() {
   this->width = 0;
   this->height = 0;
   this->filled = false;
}

void CERectangle::setWidth(unsigned int w) {
    unsigned int oldEndX = this->posX + this->width;
    unsigned int newEndX = this->posX + w;
    this->width = w;

    CEPoint *start = new CEPoint(
        (int) (oldEndX < newEndX ? oldEndX : newEndX) - 1,
        (int) this->posY
    );
    CEPoint *end = new CEPoint(
        (int) (oldEndX < newEndX ? newEndX : oldEndX) + 1,
        (int) (this->posY + this->height)
    );

    CERenderWindow *altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);
}

void CERectangle::setHeight(unsigned int h) {
    unsigned int oldEndY = this->posY + this->height;
    unsigned int newEndY = this->posY + h;
    this->height = h;

    CEPoint *start = new CEPoint(
        (int) this->posX,
        (int) (oldEndY < newEndY ? oldEndY : newEndY) - 1
    );
    CEPoint *end = new CEPoint(
        (int) (this->posX + this->width),
        (int) (oldEndY < newEndY ? newEndY : oldEndY) + 1
    );

    CERenderWindow *altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);
}

void CERectangle::setPosX(unsigned int x) {
    unsigned int oldEndX = this->posX + this->width;
    unsigned int oldStartX = this->posX;
    this->posX = x;
    unsigned int newEndX = this->posX + this->width;
    unsigned int newStartX = this->posX;

    // Adicionanando o final do retangulo
    CEPoint *start = new CEPoint(
        (int) (oldEndX < newEndX ? oldEndX : newEndX),
        (int) this->posY
    );
    CEPoint *end = new CEPoint(
        (int) (oldEndX < newEndX ? newEndX : oldEndX),
        (int) (this->posY + this->height)
    );

    CERenderWindow *altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);

    // Adicionando o inicio do retangulo
    start = new CEPoint(
        (int) (oldStartX < newStartX ? oldStartX : newStartX) - 1,
        (int) this->posY
    );
    end = new CEPoint(
        (int) (oldStartX < newStartX ? newStartX : oldStartX) + 1,
        (int) (this->posY + this->height)
    );

    altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);
}

void CERectangle::setPosY(unsigned int x) {
    unsigned int oldEndY = this->posY + this->height;
    unsigned int oldStartY = this->posY;
    this->posY = x;
    unsigned int newEndY = this->posY + this->height;
    unsigned int newStartY = this->posY;

    // Adicionanando o final do retangulo
    CEPoint *start = new CEPoint(
        (int) this->posX,
        (int) (oldEndY < newEndY ? oldEndY : newEndY)
    );
    CEPoint *end = new CEPoint(
        (int) (this->posX + this->width),
        (int) (oldEndY < newEndY ? newEndY : oldEndY)
    );

    CERenderWindow *altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);

    // Adicionando o inicio do retangulo
    start = new CEPoint(
        (int) this->posX,
        (int) (oldStartY < newStartY ? oldStartY : newStartY) - 1
    );
    end = new CEPoint(
        (int) (this->posX + this->width),
        (int) (oldStartY < newStartY ? newStartY : oldStartY) + 1
    );

    altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);
}

void CERectangle::setPos(unsigned int x, unsigned int y) {
    this->setPosX(x);
    this->setPosY(y);
}

unsigned int CERectangle::getWidth() {
    return this->width;
}

unsigned int CERectangle::getHeight() {
    return this->height;
}

CERenderWindow* CERectangle::getRenderWindow() {
    CERenderWindow *window = this->getDefaultRenderWindow();
    CEPoint *start = new CEPoint((int) this->posX, (int) this->posY);
    CEPoint *end = new CEPoint((int) (this->posX + this->width), (int) (this->posY + this->height));
    window->setPoints(start, end);
    delete start;
    delete end;
    return window;
}

void CERectangle::setFilled(bool fill) {
    this->filled = fill;
    this->addCurrentWindowAsAltered();
}

bool CERectangle::getFilled() {
    return this->filled;
}

bool CERectangle::renderToCanvas(CECanvas *canvas, CERenderWindow *window) {
    int startX = window->topLeft->x;
    int startY = window->topLeft->y;
    int endX = window->bottomRight->x;
    int endY = window->bottomRight->y;

    unsigned int cursorY = startY;
    bool allPixelRendered = true;
    while(cursorY < endY) {
        unsigned int cursorX = startX;
        while (cursorX < endX)
        {
            if(
                !this->setPixel(
                    canvas,
                    cursorX,
                    cursorY,
                    (this->filled || cursorX == startX || cursorX == (endX - 1) || cursorY == startY || cursorY == (endY - 1))
                        ? this->getBaseColor()
                        : canvas->getBackgroundColor()
                )
            ) {
                allPixelRendered = false;
            }
            cursorX++;
        }
        cursorY++;
    }
    
    return allPixelRendered;
}
