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
    int oldEndY = this->posY + (int) this->height;
    int newEndY = this->posY + (int) h;
    this->height = h;

    CEPoint *start = new CEPoint(
        this->posX,
        (oldEndY < newEndY ? oldEndY : newEndY) - 1
    );
    CEPoint *end = new CEPoint(
        this->posX + (int) this->width,
       (oldEndY < newEndY ? newEndY : oldEndY) + 1
    );

    CERenderWindow *altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);
}

void CERectangle::setPosX(int x) {
    int oldEndX = this->posX + (int) this->width;
    int oldStartX = this->posX;
    this->posX = x;
    int newEndX = this->posX + (int) this->width;
    int newStartX = this->posX;

    // Adicionanando o final do retangulo
    CEPoint *start = new CEPoint(
        (oldEndX < newEndX ? oldEndX : newEndX),
        this->posY
    );
    CEPoint *end = new CEPoint(
        (oldEndX < newEndX ? newEndX : oldEndX),
        (this->posY + (int) this->height)
    );

    CERenderWindow *altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);

    // Adicionando o inicio do retangulo
    start = new CEPoint(
        (oldStartX < newStartX ? oldStartX : newStartX) - 1,
        this->posY
    );
    end = new CEPoint(
        (oldStartX < newStartX ? newStartX : oldStartX) + 1,
        (this->posY + (int) this->height)
    );

    altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);
}

void CERectangle::setPosY(int y) {
    int oldEndY = this->posY + (int) this->height;
    int oldStartY = this->posY;
    this->posY = y;
    int newEndY = this->posY + (int) this->height;
    int newStartY = this->posY;

    // Adicionanando o final do retangulo
    CEPoint *start = new CEPoint(
        this->posX,
        (oldEndY < newEndY ? oldEndY : newEndY)
    );
    CEPoint *end = new CEPoint(
        (this->posX + (int) this->width),
        (oldEndY < newEndY ? newEndY : oldEndY)
    );

    CERenderWindow *altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);

    // Adicionando o inicio do retangulo
    start = new CEPoint(
        this->posX,
        (oldStartY < newStartY ? oldStartY : newStartY) - 1
    );
    end = new CEPoint(
        (this->posX + (int) this->width),
        (oldStartY < newStartY ? newStartY : oldStartY) + 1
    );

    altered = new CERenderWindow(start, end);
    delete start;
    delete end;

    this->addAlteredWindow(altered);
}

void CERectangle::setPos(int x, int y) {
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
    CEPoint *start = new CEPoint(this->posX, this->posY);
    CEPoint *end = new CEPoint((this->posX + (int) this->width), (this->posY + (int) this->height));
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

bool CERectangle::renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow) {
    int startX = window->topLeft->x > subWindow->topLeft->x ? window->topLeft->x : subWindow->topLeft->x;
    int startY = window->topLeft->y > subWindow->topLeft->y ? window->topLeft->y : subWindow->topLeft->y;
    int endX = window->bottomRight->x < subWindow->bottomRight->x ? window->bottomRight->x : subWindow->bottomRight->x;
    int endY = window->bottomRight->y < subWindow->bottomRight->y ? window->bottomRight->y : subWindow->bottomRight->y;

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
