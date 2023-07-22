#include "ce_circle.h"

CECircle::CECircle()
{
    this->radius = 6;
    this->filled = false;
    this->addCurrentWindowAsAltered();
}

void CECircle::setRadius(unsigned int r)
{
    this->radius = r;
    this->addCurrentWindowAsAltered();
}

unsigned int CECircle::getRadius()
{
    return this->radius;
}

CERenderWindow *CECircle::getRenderWindow()
{
    CERenderWindow *window = this->getDefaultRenderWindow();
    CEPoint *start = new CEPoint(this->posX, this->posY);
    CEPoint *end = new CEPoint(this->posX + 2 * (int)this->radius, this->posY + 2 * (int)this->radius);
    window->setPoints(start, end);
    delete start;
    delete end;
    return window;
}

void CECircle::setFilled(bool fill)
{
    this->filled = fill;
    this->addCurrentWindowAsAltered();
}

bool CECircle::getFilled()
{
    return this->filled;
}

void CECircle::setRotation(uint16_t rotation)
{
    this->rotation = 0; // Circulo não recebe rotação
}

uint16_t CECircle::getRotation()
{
    return 0;
}

bool CECircle::renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow)
{
    int startX = window->topLeft->x > subWindow->topLeft->x ? window->topLeft->x : subWindow->topLeft->x;
    int startY = window->topLeft->y > subWindow->topLeft->y ? window->topLeft->y : subWindow->topLeft->y;
    int endX = window->bottomRight->x < subWindow->bottomRight->x ? window->bottomRight->x : subWindow->bottomRight->x;
    int endY = window->bottomRight->y < subWindow->bottomRight->y ? window->bottomRight->y : subWindow->bottomRight->y;
    CEPoint *center = window->getCenterPoint();

    int cursorY = startY;
    bool allPixelRendered = true;
    while (cursorY <= endY)
    {
        int cursorX = startX;
        while (cursorX <= endX)
        {
            unsigned int distanceToCenter = center->distanceTo(CEPoint(cursorX, cursorY));
            if (distanceToCenter > this->getRadius())
            {
                cursorX++;
                continue;
            }
            if (
                !this->setPixel(
                    canvas,
                    cursorX,
                    cursorY,
                    (this->filled || distanceToCenter == (this->getRadius() - 1))
                        ? this->getBaseColor()
                        : canvas->getBackgroundColor()))
            {
                allPixelRendered = false;
            }
            cursorX++;
        }
        cursorY++;
    }

    return allPixelRendered;
}
