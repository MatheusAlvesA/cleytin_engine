#include "cleytin_engine.h"

CEActiveObject::CEActiveObject()
{
    this->graphicObject = NULL;
}

CEActiveObject::~CEActiveObject()
{
    if (this->graphicObject)
    {
        delete this->graphicObject;
    }
}

void CEActiveObject::setVisible(bool visible)
{
    if (this->graphicObject)
    {
        this->graphicObject->setVisible(visible);
    }
}

void CEActiveObject::setMirrored(bool mirrored)
{
    if (this->graphicObject)
    {
        this->graphicObject->setMirrored(mirrored);
    }
}

void CEActiveObject::setNegative(bool negative)
{
    if (this->graphicObject)
    {
        this->graphicObject->setNegative(negative);
    }
}

void CEActiveObject::setColisionEnabled(bool enabled)
{
    if (this->graphicObject)
    {
        this->graphicObject->setColisionEnabled(enabled);
    }
}

void CEActiveObject::setPriority(unsigned int priority)
{
    if (this->graphicObject)
    {
        this->graphicObject->setPriority(priority);
    }
}

void CEActiveObject::setPosX(unsigned int posX)
{
    if (this->graphicObject)
    {
        this->graphicObject->setPosX(posX);
    }
}

void CEActiveObject::setPosY(unsigned int posY)
{
    if (this->graphicObject)
    {
        this->graphicObject->setPosY(posY);
    }
}

void CEActiveObject::setMaxX(unsigned int maxX)
{
    if (this->graphicObject)
    {
        this->graphicObject->setMaxX(maxX);
    }
}

void CEActiveObject::setMaxY(unsigned int maxY)
{
    if (this->graphicObject)
    {
        this->graphicObject->setMaxY(maxY);
    }
}

unsigned int CEActiveObject::getMaxX()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getMaxX();
    }
    return 0;
}

unsigned int CEActiveObject::getMaxY()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getMaxY();
    }
    return 0;
}

void CEActiveObject::setRotation(uint16_t rotation)
{
    if (this->graphicObject)
    {
        this->graphicObject->setRotation(rotation);
    }
}

void CEActiveObject::setPos(unsigned int x, unsigned int y)
{
    if (this->graphicObject)
    {
        this->graphicObject->setPos(x, y);
    }
}

void CEActiveObject::setBaseColor(const CEColor color)
{
    if (this->graphicObject)
    {
        this->graphicObject->setBaseColor(color);
    }
}

CEColor CEActiveObject::getBaseColor()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getBaseColor();
    }
    return {0, 0, 0};
}

CERenderWindow *CEActiveObject::getRenderWindow()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getRenderWindow();
    }
    CEPoint *point = new CEPoint(0, 0);
    CERenderWindow *window = new CERenderWindow(point, point);
    delete point;
    return window;
}

bool CEActiveObject::renderToCanvas(CECanvas *canvas, CERenderWindow *window)
{
    if (this->graphicObject)
    {
        return this->graphicObject->renderToCanvas(canvas, window);
    }
    return false;
}

bool CEActiveObject::getVisible()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getVisible();
    }
    return false;
}

bool CEActiveObject::getMirrored()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getMirrored();
    }
    return false;
}

bool CEActiveObject::getNegative()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getNegative();
    }
    return false;
}

bool CEActiveObject::getColisionEnabled()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getColisionEnabled();
    }
    return false;
}

unsigned int CEActiveObject::getPriority()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getPriority();
    }
    return 0;
}

unsigned int CEActiveObject::getPosX()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getPosX();
    }
    return 0;
}

unsigned int CEActiveObject::getPosY()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getPosY();
    }
    return 0;
}

uint16_t CEActiveObject::getRotation()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getRotation();
    }
    return 0;
}

size_t CEActiveObject::getRenderWindowHeight()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getRenderWindowHeight();
    }
    return 0;
}

size_t CEActiveObject::getRenderWindowWidth()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getRenderWindowWidth();
    }
    return 0;
}

std::vector<CEPoint *> *CEActiveObject::getAllRenderWindowPoints()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getAllRenderWindowPoints();
    }
    return new std::vector<CEPoint *>;
}

bool CEActiveObject::containsPoint(CEPoint *point, unsigned int expand)
{
    if (this->graphicObject)
    {
        return this->graphicObject->containsPoint(point, expand);
    }
    return false;
}

bool CEActiveObject::containsAnyPointsFrom(std::vector<CEPoint *> *points, const unsigned int expand)
{
    if (this->graphicObject)
    {
        return this->graphicObject->containsAnyPointsFrom(points, expand);
    }
    return false;
}

CERenderWindow *CEActiveObject::getDefaultRenderWindow()
{
    if (this->graphicObject)
    {
        return this->graphicObject->getDefaultRenderWindow();
    }
    CEPoint *point = new CEPoint(0, 0);
    CERenderWindow *window = new CERenderWindow(point, point);
    window->setMaxX(0);
    window->setMaxY(0);
    return window;
}
