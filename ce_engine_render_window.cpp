#include "cleytin_engine.h"
#include "cleytin_engine_utils.h"

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
