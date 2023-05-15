#include "cleytin_engine.h"
#include "cleytin_engine_utils.cpp"

/* CleytinEngine */

CleytinEngine::CleytinEngine()
{
    this->canvas = new CECanvasTFTLCD320x240();
    this->canvas->startRender();
    this->canvas->waitRenderFinish();
}

CleytinEngine::~CleytinEngine()
{
    delete this->canvas;
}

unsigned int CleytinEngine::addObject(CEGraphicObject *obj)
{
    this->objects.push_back(obj);
    std::sort(this->objects.begin(), this->objects.end(), compareObjectPriority);
    CEActiveObject *aObj = dynamic_cast<CEActiveObject *>(obj);
    if (aObj)
    {
        aObj->setup(this);
    }
    return (unsigned int)this->objects.size();
}

bool CleytinEngine::removeObject(CEGraphicObject *obj, bool freeMemory)
{
    size_t index = this->objects.size();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        if (this->objects[i] == obj)
        {
            index = i;
            break;
        }
    }

    return this->removeObjectAt(index, freeMemory);
}

bool CleytinEngine::removeObjectAt(size_t index, bool freeMemory)
{
    if (index >= this->objects.size())
    {
        return false;
    }

    if (freeMemory)
    {
        delete this->objects[index];
    }

    while (index < this->objects.size() - 1)
    {
        this->objects[index] = this->objects[index + 1];
        index++;
    }
    this->objects.pop_back();
    return true;
}

size_t CleytinEngine::getObjectIndex(CEGraphicObject *obj)
{
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        if (obj == this->objects[i])
        {
            return i;
        }
    }
    return this->objects.size() + 1;
}

void CleytinEngine::clear(bool freeMemory)
{
    if (freeMemory)
    {
        for (size_t i = 0; i < this->objects.size(); i++)
        {
            delete this->objects[i];
        }
    }
    this->objects.clear();
}

std::vector<size_t> *CleytinEngine::getObjectsAt(CEPoint *point)
{
    std::vector<size_t> *r = new std::vector<size_t>();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        if (this->objects[i]->containsPoint(point))
        {
            r->push_back(i);
        }
    }
    return r;
}

std::vector<size_t> *CleytinEngine::getCollisionsOn(size_t index)
{
    std::vector<size_t> *r = new std::vector<size_t>();
    if (index >= this->objects.size())
    {
        return r;
    }
    CEGraphicObject *object = this->objects[index];
    if (!object->getColisionEnabled())
    {
        return r;
    }

    std::vector<CEPoint *> *targetPoints = object->getAllRenderWindowPoints();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        if (i == index || !this->objects[i]->getColisionEnabled())
        {
            continue;
        }
        std::vector<CEPoint *> *candidatePoints = this->objects[i]->getAllRenderWindowPoints();
        if (
            object->containsAnyPointsFrom(candidatePoints, 1) ||
            this->objects[i]->containsAnyPointsFrom(targetPoints, 1))
        {
            r->push_back(i);
        }
        delete_pointers_vector<CEPoint>(candidatePoints);
    }
    delete_pointers_vector<CEPoint>(targetPoints);

    return r;
}

size_t CleytinEngine::getObjectsCount()
{
    return this->objects.size();
}

CEGraphicObject *CleytinEngine::getObjectAt(size_t index)
{
    if (index >= this->objects.size())
    {
        return NULL;
    }

    return this->objects[index];
}

bool CleytinEngine::renderToCanvas()
{
    std::vector<CERenderWindow *> *alteredWindows = new std::vector<CERenderWindow *>();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        std::vector<CERenderWindow *> *currentList = this->objects[i]->getAlteredWindows();
        for (size_t i = 0; i < currentList->size(); i++)
        {
            alteredWindows->push_back(currentList->at(i)->clone());
        }
        this->objects[i]->clearAlteredWindows();
    }
    if(alteredWindows->size() <= 0) {
        delete alteredWindows;
        return false;
    }

    std::vector<CERenderWindow *> *optimizedAlteredWindows = optimize_container_windows(alteredWindows);
    delete_pointers_vector<CERenderWindow>(alteredWindows);

    printf("\r\nJanelas otimizadas\r\n");
    for(size_t i = 0; i < optimizedAlteredWindows->size(); i++) {
        CERenderWindow *window = optimizedAlteredWindows->at(i);
        printf(">>> %d\r\n", i);
        printf("TL %d %d\r\n", window->topLeft->x, window->topLeft->y);
        printf("BR %d %d\r\n", window->bottomRight->x, window->bottomRight->y);
        printf("--------------\r\n");
    }
    //TODO, renderizar só as janelas

    this->canvas->clear();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        this->objects[i]->renderToCanvas(this->canvas);
    }

    delete_pointers_vector<CERenderWindow>(optimizedAlteredWindows);
    return true;
}

uint64_t CleytinEngine::render()
{
    uint64_t start = esp_timer_get_time();
    this->canvas->waitRenderFinish(); // Finalize o render anterior caso não tenha
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        CEActiveObject *obj = dynamic_cast<CEActiveObject *>(this->objects[i]);
        if (obj)
        {
            obj->beforeRender(this);
        }
    }
    bool needRender = this->renderToCanvas();
    if(needRender) {
        this->canvas->startRender();
    }
    return esp_timer_get_time() - start;
}

uint64_t CleytinEngine::waitRender()
{
    uint64_t start = esp_timer_get_time();
    this->canvas->waitRenderFinish();
    return esp_timer_get_time() - start;
}

uint64_t CleytinEngine::renderSync()
{
    uint64_t renderTime = this->render();
    uint64_t sendTime = this->waitRender();
    return renderTime + sendTime;
}

uint64_t CleytinEngine::loop()
{
    uint64_t start = esp_timer_get_time();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        CEActiveObject *obj = dynamic_cast<CEActiveObject *>(this->objects[i]);
        if (obj)
        {
            obj->beforeLoop(this);
        }
    }
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        CEActiveObject *obj = dynamic_cast<CEActiveObject *>(this->objects[i]);
        if (obj)
        {
            obj->loop(this);
        }
    }
    uint64_t end = esp_timer_get_time();
    return end - start;
}

/* CEPoint */

CEPoint::CEPoint(int x, int y)
{
    this->x = x;
    this->y = y;
}

bool CEPoint::operator==(const CEPoint &dot)
{
    return this->x == dot.x && this->y == dot.y;
}

bool CEPoint::operator!=(const CEPoint &dot)
{
    return this->x != dot.x || this->y != dot.y;
}

void CEPoint::rotate(CEPoint *rotationCenter, uint16_t degrees)
{
    if (degrees == 0)
    {
        return;
    }

    int normX = this->x - rotationCenter->x;
    int normY = this->y - rotationCenter->y;

    this->x = (normX * cosLookUp[degrees] - normY * sinLookUp[degrees]) + rotationCenter->x;
    this->y = (normX * sinLookUp[degrees] + normY * cosLookUp[degrees]) + rotationCenter->y;
}

CEPoint *CEPoint::clone()
{
    return new CEPoint(this->x, this->y);
}

unsigned int CEPoint::distanceTo(const CEPoint point)
{
    int dX = this->x - point.x;
    int dY = this->y - point.y;

    return (unsigned int)round(sqrt(dX * dX + dY * dY));
}

/* CELine */

CELine::CELine(const CEPoint &start, const CEPoint &end)
{
    this->start = new CEPoint(start.x, start.y);
    this->end = new CEPoint(end.x, end.y);
}

CELine::~CELine()
{
    delete this->start;
    delete this->end;
}

int CELine::calculateSideOfPoint(CEPoint *point)
{
    return (point->y - this->start->y) * (this->end->x - this->start->x) - (point->x - this->start->x) * (this->end->y - this->start->y);
}

int CELine::getSize()
{
    return this->start->distanceTo(*this->end);
}

/* CERenderWindow */

CERenderWindow::CERenderWindow(const CEPoint *start, const CEPoint *end)
{
    this->topLeft = NULL;
    this->bottomRight = NULL;
    this->topRight = NULL;
    this->bottomLeft = NULL;
    this->setPoints(start, end);
    this->maxX = LCD_WIDTH_PIXELS;
    this->maxY = LCD_HEIGHT_PIXELS;
}

bool CERenderWindow::operator==(const CERenderWindow &window)
{
    return this->topLeft == window.topLeft &&
           this->bottomRight == window.bottomRight &&
           this->topRight == window.topRight &&
           this->bottomLeft == window.bottomLeft;
}

bool CERenderWindow::operator!=(const CERenderWindow &window)
{
    return this->topLeft != window.topLeft ||
           this->bottomRight != window.bottomRight ||
           this->topRight != window.topRight ||
           this->bottomLeft != window.bottomLeft;
}

void CERenderWindow::setMaxX(unsigned int x)
{
    this->maxX = x;
}

void CERenderWindow::setMaxY(unsigned int y)
{
    this->maxY = y;
}

bool CERenderWindow::isZeroSize() {
    CELine *topLine = this->getTopLine();
    CELine *bottomLine = this->getBottomLine();
    CELine *leftLine = this->getLeftLine();
    CELine *rightLine = this->getRightLine();

    bool r = false;
    if(
        topLine->getSize() == 0 &&
        bottomLine->getSize() == 0 &&
        leftLine->getSize() == 0 &&
        rightLine->getSize() == 0
    ) {
        r = true;
    }

    delete topLine;
    delete bottomLine;
    delete leftLine;
    delete rightLine;

    return r;
}

void CERenderWindow::setPoints(const CEPoint *start, const CEPoint *end)
{
    if (this->topLeft)
    {
        delete this->topLeft;
    }
    this->topLeft = new CEPoint(start->x, start->y);

    if (this->bottomRight)
    {
        delete this->bottomRight;
    }
    this->bottomRight = new CEPoint(end->x, end->y);

    if (this->topRight)
    {
        delete this->topRight;
    }
    this->topRight = new CEPoint(end->x, start->y);

    if (this->bottomLeft)
    {
        delete this->bottomLeft;
    }
    this->bottomLeft = new CEPoint(start->x, end->y);
}

CERenderWindow::~CERenderWindow()
{
    delete this->topLeft;
    delete this->topRight;
    delete this->bottomLeft;
    delete this->bottomRight;
}

CEPoint *CERenderWindow::getCenterPoint()
{
    return new CEPoint(
        ((this->bottomRight->x - this->topLeft->x) / 2) + this->topLeft->x,
        ((this->bottomRight->y - this->topLeft->y) / 2) + this->topLeft->y);
}

CELine *CERenderWindow::getTopLine()
{
    CEPoint *start = new CEPoint(this->topRight->x, this->topRight->y);
    CEPoint *end = new CEPoint(this->topLeft->x, this->topLeft->y);
    CELine *r = new CELine(*start, *end);
    delete start;
    delete end;
    return r;
}

CELine *CERenderWindow::getBottomLine()
{
    CEPoint *start = new CEPoint(this->bottomLeft->x, this->bottomLeft->y);
    CEPoint *end = new CEPoint(this->bottomRight->x, this->bottomRight->y);
    CELine *r = new CELine(*start, *end);
    delete start;
    delete end;
    return r;
}

CELine *CERenderWindow::getLeftLine()
{
    CEPoint *start = new CEPoint(this->topLeft->x, this->topLeft->y);
    CEPoint *end = new CEPoint(this->bottomLeft->x, this->bottomLeft->y);
    CELine *r = new CELine(*start, *end);
    delete start;
    delete end;
    return r;
}

CELine *CERenderWindow::getRightLine()
{
    CEPoint *start = new CEPoint(this->bottomRight->x, this->bottomRight->y);
    CEPoint *end = new CEPoint(this->topRight->x, this->topRight->y);
    CELine *r = new CELine(*start, *end);
    delete start;
    delete end;
    return r;
}

void CERenderWindow::rotate(uint16_t degrees)
{
    if (degrees == 0)
    {
        return;
    }

    CEPoint *center = this->getCenterPoint();

    this->topLeft->rotate(center, degrees);
    this->topRight->rotate(center, degrees);
    this->bottomLeft->rotate(center, degrees);
    this->bottomRight->rotate(center, degrees);
    delete center;
}

void CERenderWindow::resetToStartPosition()
{
    int offsetX = this->topLeft->x;
    int offsetY = this->topLeft->y;
    this->topLeft->x -= offsetX;
    this->topLeft->y -= offsetY;
    this->topRight->x -= offsetX;
    this->topRight->y -= offsetY;
    this->bottomLeft->x -= offsetX;
    this->bottomLeft->y -= offsetY;
    this->bottomRight->x -= offsetX;
    this->bottomRight->y -= offsetY;
}

bool CERenderWindow::containsPoint(CEPoint *point)
{
    CELine *topLine = this->getTopLine();
    CELine *bottomLine = this->getBottomLine();
    CELine *leftLine = this->getLeftLine();
    CELine *rightLine = this->getRightLine();

    int relativeTop = topLine->calculateSideOfPoint(point);
    int relativeBottom = bottomLine->calculateSideOfPoint(point);
    int relativeLeft = leftLine->calculateSideOfPoint(point);
    int relativeRight = rightLine->calculateSideOfPoint(point);

    bool r = false;
    if (
        (
            relativeTop >= 0 &&
            relativeBottom >= 0 &&
            relativeLeft >= 0 &&
            relativeRight >= 0) ||
        (relativeTop <= 0 &&
         relativeBottom <= 0 &&
         relativeLeft <= 0 &&
         relativeRight <= 0))
    {
        r = true;
    }

    delete topLine;
    delete bottomLine;
    delete leftLine;
    delete rightLine;

    return r;
}

bool CERenderWindow::containsWindow(CERenderWindow *window) {
    return this->containsPoint(window->topLeft) &&
        this->containsPoint(window->topRight) &&
        this->containsPoint(window->bottomLeft) &&
        this->containsPoint(window->bottomRight);
}

CERenderWindow *CERenderWindow::clone() {
    CERenderWindow *r = new CERenderWindow(this->topLeft, this->bottomRight);
    if(this->topLeft) {
        r->topLeft = this->topLeft->clone();
    } else {
        r->topLeft = nullptr;
    }
    if(this->topRight) {
        r->topRight = this->topRight->clone();
    } else {
        r->topRight = nullptr;
    }
    if(this->bottomLeft) {
        r->bottomLeft = this->bottomLeft->clone();
    } else {
        r->bottomLeft = nullptr;
    }
    if(this->bottomRight) {
        r->bottomRight = this->bottomRight->clone();
    } else {
        r->bottomRight = nullptr;
    }
    r->setMaxX(this->maxX);
    r->setMaxY(this->maxY);
    
    return r;
}

void CERenderWindow::expand(unsigned int size)
{
    if (size >= this->maxY || size >= this->maxX)
    {
        return;
    }
    this->topLeft->x -= this->topLeft->x >= size ? size : 0;
    this->topLeft->y -= this->topLeft->y >= size ? size : 0;
    this->topRight->x += this->topRight->x < (this->maxX - size) ? size : 0;
    this->topRight->y -= this->topRight->y >= size ? size : 0;
    this->bottomLeft->x -= this->bottomLeft->x >= size ? size : 0;
    this->bottomLeft->y += this->bottomLeft->y < (this->maxY - size) ? size : 0;
    this->bottomRight->x += this->bottomRight->x < (this->maxX - size) ? size : 0;
    this->bottomRight->y += this->bottomRight->y < (this->maxY - size) ? size : 0;
}

std::vector<CEPoint *> *CERenderWindow::getAllPoints()
{
    std::vector<CEPoint *> *points = new std::vector<CEPoint *>();
    points->push_back(this->topLeft->clone());
    points->push_back(this->topRight->clone());
    points->push_back(this->bottomLeft->clone());
    points->push_back(this->bottomRight->clone());
    return points;
}

size_t CERenderWindow::getHeight()
{
    std::vector<CEPoint *> *points = this->getAllPoints();
    int min = points->at(0)->y;
    int max = points->at(0)->y;
    for (size_t i = 0; i < points->size(); i++)
    {
        if (points->at(i)->y < min)
        {
            min = points->at(i)->y;
        }
        if (points->at(i)->y > max)
        {
            max = points->at(i)->y;
        }
    }
    delete_pointers_vector<CEPoint>(points);
    return (size_t)(max - min);
}

size_t CERenderWindow::getWidth()
{
    std::vector<CEPoint *> *points = this->getAllPoints();
    int min = points->at(0)->x;
    int max = points->at(0)->x;
    for (size_t i = 0; i < points->size(); i++)
    {
        if (points->at(i)->x < min)
        {
            min = points->at(i)->x;
        }
        if (points->at(i)->x > max)
        {
            max = points->at(i)->x;
        }
    }
    delete_pointers_vector<CEPoint>(points);
    return (size_t)(max - min);
}

/* CEGraphicObject */

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

/* CEActiveObject */

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
