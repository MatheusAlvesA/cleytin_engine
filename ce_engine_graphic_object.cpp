#include "cleytin_engine.h"
#include "cleytin_engine_utils.h"

CEGraphicObject::CEGraphicObject()
{
    this->colisionEnabled = true;
    this->visible = true;
    this->mirrored = false;
    this->pixelShaderBeforeEfects = NULL;
    this->priority = 0;
    this->posX = 0;
    this->posY = 0;
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

void CEGraphicObject::setPixelShaderBeforeEfects(std::function<void(CEGraphicObject*, int, int, CEColor *)> callback) {
    this->pixelShaderBeforeEfects = callback;
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

void CEGraphicObject::setPosX(int posX)
{
    if(posX == this->posX) return;
    this->addCurrentWindowAsAltered();
    this->posX = posX;
    this->addCurrentWindowAsAltered();
}

void CEGraphicObject::setPosY(int posY)
{
    if(posY == this->posY) return;
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

void CEGraphicObject::setPos(int x, int y)
{
    if(x == this->posX && y == this->posY) return;
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

void CEGraphicObject::renderToCanvas(CECanvas *canvas, CERenderWindow *subWindow)
{
    if (!this->getVisible())
    {
        return;
    }
    CERenderWindow *w = this->getRenderWindow();
    this->renderToCanvas(canvas, w, subWindow);
    delete w;
}

bool CEGraphicObject::getVisible()
{
    return this->visible;
}

bool CEGraphicObject::getMirrored()
{
    return this->mirrored;
}

bool CEGraphicObject::getColisionEnabled()
{
    return this->colisionEnabled;
}

unsigned int CEGraphicObject::getPriority()
{
    return this->priority;
}

int CEGraphicObject::getPosX()
{
    return this->posX;
}

int CEGraphicObject::getPosY()
{
    return this->posY;
}

size_t CEGraphicObject::getRenderWindowHeight()
{
    CERenderWindow *w = this->getRenderWindow();
    size_t size = w->getHeight();
    delete w;
    return size;
}

size_t CEGraphicObject::getRenderWindowWidth()
{
    CERenderWindow *w = this->getRenderWindow();
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

bool CEGraphicObject::setPixel(CECanvas *canvas, int x, int y, CEColor color)
{
    if (x < 0 || y < 0) return false;

    if(this->pixelShaderBeforeEfects != NULL) {
        this->pixelShaderBeforeEfects(this, posX, posY, &color);
    }

    if (this->getMirrored())
    {
        this->mirrorPixel(x);
    }

    unsigned int posX = (unsigned int)x;
    unsigned int posY = (unsigned int)y;

    canvas->setPixel(posX, posY, color);

    return true;
}

bool CEGraphicObject::setPixel(CECanvas *canvas, int x, int y, uint16_t color)
{
    if (x < 0 || y < 0) return false;

    if(this->pixelShaderBeforeEfects != NULL) {
        CEColor colorStruct = RGB565ToColor(color);
        this->pixelShaderBeforeEfects(this, posX, posY, &colorStruct);
        color = colorToRGB565(colorStruct);
    }

    if (this->getMirrored())
    {
        this->mirrorPixel(x);
    }

    unsigned int posX = (unsigned int)x;
    unsigned int posY = (unsigned int)y;

    canvas->setPixel(posX, posY, color);

    return true;
}

bool CEGraphicObject::containsPoint(CEPoint *point, unsigned int expand)
{
    CERenderWindow *w = this->getRenderWindow();
    w->expand(expand);
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

void CEGraphicObject::addCurrentWindowAsAltered()
{
    CERenderWindow *w = this->getRenderWindow();
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
    this->addAlteredWindow(w);
}

bool CEGraphicObject::addAlteredWindow(CERenderWindow *w) {
    if(w->topLeft->x < 0) {
        w->topLeft->x = 0;
    }
    if(w->topLeft->y < 0) {
        w->topLeft->y = 0;
    }
    if(w->bottomRight->x > this->maxX) {
        w->bottomRight->x = this->maxX;
    }
    if(w->bottomRight->y > this->maxY) {
        w->bottomRight->y = this->maxY;
    }

    if(
        w->topLeft->x > this->maxX ||
        w->topLeft->y > this->maxY ||
        w->bottomRight->x < 0 ||
        w->bottomRight->y < 0 ||
        w->topLeft->x > w->bottomRight->x ||
        w->topLeft->y > w->bottomRight->y
    ) {
        delete w;
        return false;
    }

    this->alteredWindows->push_back(w);
    return true;
}

void CEGraphicObject::clearAlteredWindows()
{
    for (size_t i = 0; i < this->alteredWindows->size(); i++)
    {
        delete this->alteredWindows->at(i);
    }
    this->alteredWindows->clear();
}

void CEGraphicObject::setup(CleytinEngine *engine) {/* EMPTY */}
void CEGraphicObject::beforeLoop(CleytinEngine *engine) {/* EMPTY */}
void CEGraphicObject::loop(CleytinEngine *engine) {/* EMPTY */}
void CEGraphicObject::beforeRender(CleytinEngine *engine) {/* EMPTY */}
void CEGraphicObject::beforeRemove(CleytinEngine *engine) {/* EMPTY */}
