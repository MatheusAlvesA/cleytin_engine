#include "cleytin_engine.h"
#include "cleytin_engine_utils.h"

CEGraphicObject::CEGraphicObject()
{
    this->colisionEnabled = true;
    this->visible = true;
    this->mirrored = false;
    this->negative = false;
    this->priority = 0;
    this->posX = 0;
    this->posY = 0;
    this->rotation = 0;
    this->maxX = LCD_WIDTH_PIXELS;
    this->maxY = LCD_HEIGHT_PIXELS;
    this->baseColor = {0, 0, 0};
    this->alteredWindows = new std::vector<CERenderWindow *>();
}

CEGraphicObject::~CEGraphicObject()
{
    for (size_t i = 0; i < this->alteredWindows->size(); i++)
    {
        delete this->alteredWindows->at(i);
    }
    delete this->alteredWindows;
}

void CEGraphicObject::setVisible(bool visible)
{
    this->visible = visible;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setMirrored(bool mirrored)
{
    this->mirrored = mirrored;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setNegative(bool negative)
{
    this->negative = negative;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setColisionEnabled(bool enabled)
{
    this->colisionEnabled = enabled;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setPriority(unsigned int priority)
{
    this->priority = priority;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setPosX(unsigned int posX)
{
    this->addCurrentWindowAsAltered();
    this->posX = posX;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setPosY(unsigned int posY)
{
    this->addCurrentWindowAsAltered();
    this->posY = posY;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setMaxX(unsigned int maxX)
{
    this->maxX = maxX;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setMaxY(unsigned int maxY)
{
    this->maxY = maxY;
    this->addCurrentWindowAsAltered();
}

unsigned int CEGraphicObject::getMaxX()
{
    return this->maxX;
}

unsigned int CEGraphicObject::getMaxY()
{
    return this->maxY;
}

void CEGraphicObject::setRotation(uint16_t rotation)
{
    this->rotation = rotation % 360;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setPos(unsigned int x, unsigned int y)
{
    this->addCurrentWindowAsAltered();
    this->posX = x;
    this->posY = y;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setBaseColor(const CEColor color)
{
    this->baseColor = color;
    this->addCurrentWindowAsAltered();
}

CEColor CEGraphicObject::getBaseColor()
{
    return this->baseColor;
}

bool CEGraphicObject::renderToCanvas(CECanvas *canvas)
{
    if (!this->getVisible())
    {
        return true;
    }
    CERenderWindow *w = this->getRenderWindow();
    bool result = this->renderToCanvas(canvas, w);
    delete w;
    return result;
}

bool CEGraphicObject::getVisible()
{
    return this->visible;
}

bool CEGraphicObject::getMirrored()
{
    return this->mirrored;
}

bool CEGraphicObject::getNegative()
{
    return this->negative;
}

bool CEGraphicObject::getColisionEnabled()
{
    return this->colisionEnabled;
}

unsigned int CEGraphicObject::getPriority()
{
    return this->priority;
}

unsigned int CEGraphicObject::getPosX()
{
    return this->posX;
}

unsigned int CEGraphicObject::getPosY()
{
    return this->posY;
}

uint16_t CEGraphicObject::getRotation()
{
    return this->rotation;
}

size_t CEGraphicObject::getRenderWindowHeight()
{
    CERenderWindow *w = this->getRenderWindow();
    w->rotate(this->getRotation());
    size_t size = w->getHeight();
    delete w;
    return size;
}

size_t CEGraphicObject::getRenderWindowWidth()
{
    CERenderWindow *w = this->getRenderWindow();
    w->rotate(this->getRotation());
    size_t size = w->getWidth();
    delete w;
    return size;
}

void CEGraphicObject::mirrorPixel(int &x)
{
    CERenderWindow *w = this->getRenderWindow();
    CEPoint *c = w->getCenterPoint();
    int centerX = c->x;
    delete w;
    delete c;

    x = (centerX - x) + centerX;
}

std::vector<CEPoint *> *CEGraphicObject::getAllRenderWindowPoints()
{
    CERenderWindow *w = this->getRenderWindow();
    w->rotate(this->getRotation());
    std::vector<CEPoint *> *r = w->getAllPoints();
    delete w;

    return r;
}

bool CEGraphicObject::rotatePixel(int &x, int &y, uint16_t rot)
{
    if (rot == 0)
    {
        if (x >= this->maxX || y >= this->maxY)
        {
            return false;
        }
        return true;
    }

    CERenderWindow *w = this->getRenderWindow();
    CEPoint *center = w->getCenterPoint();
    delete w;

    int normX = x - center->x;
    int normY = y - center->y;

    int newX = (normX * cosLookUp[rot] - normY * sinLookUp[rot]) + center->x;
    int newY = (normX * sinLookUp[rot] + normY * cosLookUp[rot]) + center->y;
    delete center;

    if (
        newX < 0 || newX >= this->maxX ||
        newY < 0 || newY >= this->maxY)
    {
        return false;
    }

    x = newX;
    y = newY;

    return true;
}

bool CEGraphicObject::setPixel(CECanvas *canvas, int x, int y, CEColor color)
{
    if (this->getMirrored())
    {
        this->mirrorPixel(x);
    }
    if (!this->rotatePixel(x, y, this->getRotation()))
    {
        return false;
    }
    if (x < 0 || y < 0)
    {
        return false;
    }
    unsigned int posX = (unsigned int)x;
    unsigned int posY = (unsigned int)y;

    if (this->getNegative())
    {
        color = -color;
    }

    canvas->setPixel(posX, posY, color);

    return true;
}

bool CEGraphicObject::containsPoint(CEPoint *point, unsigned int expand)
{
    CERenderWindow *w = this->getRenderWindow();
    w->expand(expand);
    w->rotate(this->getRotation());
    bool r = w->containsPoint(point);
    delete w;
    return r;
}

bool CEGraphicObject::containsAnyPointsFrom(std::vector<CEPoint *> *points, const unsigned int expand)
{
    for (size_t i = 0; i < points->size(); i++)
    {
        if (this->containsPoint((*points)[i], expand))
        {
            return true;
        }
    }
    return false;
}

CERenderWindow *CEGraphicObject::getDefaultRenderWindow()
{
    CEPoint *point = new CEPoint(this->posX, this->posY);
    CERenderWindow *window = new CERenderWindow(point, point);
    window->setMaxX(this->maxX);
    window->setMaxY(this->maxY);
    delete point;
    return window;
}

std::vector<CERenderWindow *> *CEGraphicObject::getAlteredWindows()
{
    return this->alteredWindows;
}

CERenderWindow *CEGraphicObject::getContainingWindow()
{
    CERenderWindow *w = this->getRenderWindow();
    w->rotate(this->getRotation());
    int minX = w->topLeft->x;
    int minY = w->topLeft->y;
    int maxX = w->topLeft->x;
    int maxY = w->topLeft->y;
    std::vector<CEPoint *> *points = w->getAllPoints();
    for (size_t i = 0; i < points->size(); i++)
    {
        if ((*points)[i]->x < minX)
        {
            minX = (*points)[i]->x;
        }
        if ((*points)[i]->y < minY)
        {
            minY = (*points)[i]->y;
        }
        if ((*points)[i]->x > maxX)
        {
            maxX = (*points)[i]->x;
        }
        if ((*points)[i]->y > maxY)
        {
            maxY = (*points)[i]->y;
        }
    }
    delete_pointers_vector<CEPoint>(points);

    CEPoint *topLeft = new CEPoint(minX, minY);
    CEPoint *bottomRight = new CEPoint(maxX, maxY);
    w->setPoints(topLeft, bottomRight);
    delete topLeft;
    delete bottomRight;
    return w;
}

void CEGraphicObject::addCurrentWindowAsAltered()
{
    CERenderWindow *w = this->getContainingWindow();
    if(w->isZeroSize()) {
        delete w;
        return;
    }
    for (size_t i = 0; i < this->alteredWindows->size(); i++)
    {
        if(*this->alteredWindows->at(i) == *w) {
            delete w;
            return;
        }

        if (this->alteredWindows->at(i)->containsWindow(w))
        {
            delete w;
            return;
        }
    }
    this->alteredWindows->push_back(w);
}

void CEGraphicObject::clearAlteredWindows()
{
    for (size_t i = 0; i < this->alteredWindows->size(); i++)
    {
        delete this->alteredWindows->at(i);
    }
    this->alteredWindows->clear();
}