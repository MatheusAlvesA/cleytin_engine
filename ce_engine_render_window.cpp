#include "cleytin_engine.h"
#include "cleytin_engine_utils.h"

CERenderWindow::CERenderWindow(const CEPoint *start, const CEPoint *end)
{
    this->topLeft = NULL;
    this->bottomRight = NULL;
    this->setPoints(start, end);
    this->maxX = LCD_WIDTH_PIXELS;
    this->maxY = LCD_HEIGHT_PIXELS;
}

bool CERenderWindow::operator==(const CERenderWindow &window)
{
    return this->topLeft == window.topLeft &&
           this->bottomRight == window.bottomRight;
}

bool CERenderWindow::operator!=(const CERenderWindow &window)
{
    return this->topLeft != window.topLeft ||
           this->bottomRight != window.bottomRight;
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
    return this->topLeft->x == this->bottomRight->x &&
        this->topLeft->y == this->bottomRight->y;
}

/**
 * @brief Esse método retorna o lado da janela passada que tem uma parte contida dentro da janela atual.
 * 
 * @param window
 * @return CEWindowIntersectionSide
*/
CEWindowIntersectionSide CERenderWindow::getContainingSide(CERenderWindow *window) {
    CEPoint tr(window->bottomRight->x, window->topLeft->y);
    CEPoint bl(window->topLeft->x, window->bottomRight->y);
    if(this->containsPoint(window->topLeft) && this->containsPoint(&tr)) {
        return CEWindowIntersectionSide::TOP;
    }
    if(this->containsPoint(&bl) && this->containsPoint(window->bottomRight)) {
        return CEWindowIntersectionSide::BOTTOM;
    }
    if(this->containsPoint(window->topLeft) && this->containsPoint(&bl)) {
        return CEWindowIntersectionSide::LEFT;
    }
    if(this->containsPoint(&tr) && this->containsPoint(window->bottomRight)) {
        return CEWindowIntersectionSide::RIGHT;
    }
    return CEWindowIntersectionSide::NONE;
}

bool CERenderWindow::doOverlap(CERenderWindow *window)
{
    // Se um está à direita do outro
    if (
        this->topLeft->x > window->bottomRight->x ||
        window->topLeft->x > this->bottomRight->x
    ) {
        return false;
    }
 
    // Se um está acima do outro
    if (
        this->bottomRight->y < window->topLeft->y ||
        window->bottomRight->y < this->topLeft->y
    ) {
        return false;
    }
 
    return true;
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

    if (this->topLeft->x > this->bottomRight->x)
    {
        unsigned int tlX = this->topLeft->x;
        this->topLeft->x = this->bottomRight->x;
        this->bottomRight->x = tlX;
    }

    if (this->topLeft->y > this->bottomRight->y)
    {
        unsigned int tlY = this->topLeft->y;
        this->topLeft->y = this->bottomRight->y;
        this->bottomRight->y = tlY;
    }
}

CERenderWindow::~CERenderWindow()
{
    delete this->topLeft;
    delete this->bottomRight;
}

CEPoint *CERenderWindow::getCenterPoint()
{
    return new CEPoint(
        ((this->bottomRight->x - this->topLeft->x) / 2) + this->topLeft->x,
        ((this->bottomRight->y - this->topLeft->y) / 2) + this->topLeft->y);
}

void CERenderWindow::resetToStartPosition()
{
    this->bottomRight->x -= this->topLeft->x;
    this->bottomRight->y -= this->topLeft->y;
    this->topLeft->x = 0;
    this->topLeft->y = 0;
}

bool CERenderWindow::containsPoint(CEPoint *point)
{
    return point->x >= this->topLeft->x &&
           point->x <= this->bottomRight->x &&
           point->y <= this->bottomRight->y &&
           point->y >= this->topLeft->y;
}

bool CERenderWindow::containsWindow(CERenderWindow *window) {
    return this->containsPoint(window->topLeft) && this->containsPoint(window->bottomRight);
}

CERenderWindow *CERenderWindow::clone() {
    CERenderWindow *r = new CERenderWindow(this->topLeft, this->bottomRight);
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
    this->bottomRight->x += this->bottomRight->x < (this->maxX - size) ? size : 0;
    this->bottomRight->y += this->bottomRight->y < (this->maxY - size) ? size : 0;
}

size_t CERenderWindow::getHeight()
{
    return this->bottomRight->y - this->topLeft->y;
}

size_t CERenderWindow::getWidth()
{
    return this->bottomRight->x - this->topLeft->x;
}

size_t CERenderWindow::getArea()
{
    return this->getHeight() * this->getWidth();
}
