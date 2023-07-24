#include "ce_colorful_bitmap.h"

CEColorfulBitmap::CEColorfulBitmap()
{
    this->width = 0;
    this->height = 0;
    this->buffer = NULL;
    this->alphaColor = 0;
    this->sizeMultiplier = 1;
    this->addCurrentWindowAsAltered();
}

void CEColorfulBitmap::setWidth(unsigned int w)
{
    this->width = w;
    this->addCurrentWindowAsAltered();
}

void CEColorfulBitmap::setHeight(unsigned int h)
{
    this->height = h;
    this->addCurrentWindowAsAltered();
}

void CEColorfulBitmap::setSizeMultiplier(uint8_t size)
{
    this->sizeMultiplier = size;
    this->addCurrentWindowAsAltered();
}

uint8_t CEColorfulBitmap::getSizeMultiplier()
{
    return this->sizeMultiplier;
}

void CEColorfulBitmap::setAlphaColor(uint16_t color)
{
    this->alphaColor = color;
    this->addCurrentWindowAsAltered();
}

unsigned int CEColorfulBitmap::getWidth()
{
    return this->width * this->sizeMultiplier;
}

unsigned int CEColorfulBitmap::getHeight()
{
    return this->height * this->sizeMultiplier;
}

uint16_t CEColorfulBitmap::getAlphaColor()
{
    return this->alphaColor;
}

CERenderWindow *CEColorfulBitmap::getRenderWindow()
{
    CERenderWindow *window = this->getDefaultRenderWindow();
    CEPoint *start = new CEPoint(this->posX, this->posY);
    CEPoint *end = new CEPoint(this->posX + (int)this->getWidth(), this->posY + (int)this->getHeight());
    window->setPoints(start, end);
    delete start;
    delete end;
    return window;
}

void CEColorfulBitmap::setBuffer(const uint16_t *buffer)
{
    this->buffer = buffer;
    this->addCurrentWindowAsAltered();
}

const uint16_t *CEColorfulBitmap::getBuffer()
{
    return this->buffer;
}

bool CEColorfulBitmap::renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow)
{
    int startX = window->topLeft->x > subWindow->topLeft->x ? window->topLeft->x : subWindow->topLeft->x;
    int startY = window->topLeft->y > subWindow->topLeft->y ? window->topLeft->y : subWindow->topLeft->y;
    int endX = window->bottomRight->x < subWindow->bottomRight->x ? window->bottomRight->x : subWindow->bottomRight->x;
    int endY = window->bottomRight->y < subWindow->bottomRight->y ? window->bottomRight->y : subWindow->bottomRight->y;

    unsigned int offsetX = (unsigned int) (startX - window->topLeft->x) / (unsigned int) this->getSizeMultiplier();
    unsigned int offsetY = (unsigned int) (startY - window->topLeft->y) / (unsigned int) this->getSizeMultiplier();

    int cursorY = startY - ((startY - window->topLeft->y) % this->getSizeMultiplier());
    unsigned int internalCursorY = offsetY;
    bool allPixelsRendered = true;
    while (cursorY < endY)
    {
        int cursorX = startX - ((startX - window->topLeft->x) % this->getSizeMultiplier());
        unsigned int internalCursorX = offsetX;
        while (cursorX < endX)
        {
            unsigned int index = internalCursorX + (internalCursorY * this->width);
            for (size_t i = 0; i < this->getSizeMultiplier(); i++)
            {
                for (size_t j = 0; j < this->getSizeMultiplier(); j++)
                {
                    if(this->buffer[index] == this->alphaColor) {
                        continue;
                    }
                    if (
                        !this->setPixel(
                            canvas,
                            i + cursorX,
                            j + cursorY,
                            this->buffer[index]
                        ))
                    {
                        allPixelsRendered = false;
                    }
                }
            }
            cursorX += this->getSizeMultiplier();
            internalCursorX++;
        }
        cursorY += this->getSizeMultiplier();
        internalCursorY++;
    }

    return allPixelsRendered;
}
