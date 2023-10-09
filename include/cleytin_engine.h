#ifndef CLEYTIN_ENGINE_H
#define CLEYTIN_ENGINE_H

#include <vector>
#include <algorithm>
#include <stddef.h>
#include "esp_system.h"
#include "ce_canvas_tftlcd_320x240.h"
#include "ce_graphic_object.h"
#include "esp_timer.h"

#include <math.h>

#define PI 3.142857

typedef enum CEWindowIntersectionSide
{
    TOP = 1,
    BOTTOM = -1,
    NONE = 0,
    LEFT = -2,
    RIGHT = 2
} CEWindowIntersectionSide;

class CEPoint
{
public:
    int x;
    int y;

    CEPoint(int x, int y);
    bool operator==(const CEPoint &dot);
    bool operator!=(const CEPoint &dot);

    void rotate(CEPoint *rotationCenter, uint16_t degrees);
    CEPoint *clone();
    unsigned int distanceTo(const CEPoint point);
};

class CELine
{
public:
    CELine(const CEPoint &start, const CEPoint &end);
    ~CELine();
    CEPoint *start;
    CEPoint *end;

    int calculateSideOfPoint(CEPoint *point);
    int getSize();
    CELine *clone();
};

class CERenderWindow
{
public:
    CERenderWindow(const CEPoint *start, const CEPoint *end);
    ~CERenderWindow();
    CEPoint *topLeft;
    CEPoint *topRight;
    CEPoint *bottomLeft;
    CEPoint *bottomRight;

    void setPoints(const CEPoint *start, const CEPoint *end);
    void setMaxX(unsigned int x);
    void setMaxY(unsigned int y);

    CEPoint *getCenterPoint();
    CELine *getTopLine();
    CELine *getBottomLine();
    CELine *getLeftLine();
    CELine *getRightLine();
    size_t getHeight();
    size_t getWidth();

    std::vector<CEPoint *> *getAllPoints();

    bool containsPoint(CEPoint *point);
    bool containsWindow(CERenderWindow *window);
    void expand(unsigned int size);
    CERenderWindow *clone();

    void rotate(uint16_t degrees);
    void resetToStartPosition();

    bool isZeroSize();

    CEWindowIntersectionSide getContainingSide(CERenderWindow *window);
    bool doOverlapRotated(CERenderWindow *window);
    bool doOverlapNotRotated(CERenderWindow *window);

    bool operator==(const CERenderWindow &window);
    bool operator!=(const CERenderWindow &window);

private:
    unsigned int maxX;
    unsigned int maxY;
};

class CleytinEngine
{
public:
    CleytinEngine();
    ~CleytinEngine();

    unsigned int addObject(CEGraphicObject *obj);
    /**
     * @brief Marca um objeto para ser apagado(memória liberada) no início do próximo loop, garantindo que o objeto não será apagado durante o loop atual.
     */
    void markToDelete(CEGraphicObject *obj);
    bool removeObject(CEGraphicObject *obj, bool freeMemory = false);
    bool removeObjectAt(size_t index, bool freeMemory = false);
    void clear(bool freeMemory = false);
    std::vector<size_t> *getCollisionsOn(size_t index);
    std::vector<size_t> *getCollisionsOn(CEGraphicObject *obj);
    CEGraphicObject *getObjectAt(size_t index);
    std::vector<size_t> *getObjectsAt(CEPoint *point);
    size_t getObjectIndex(CEGraphicObject *obj);
    size_t getObjectsCount();
    std::vector<CEGraphicObject *> *getObjectsOnWindow(CERenderWindow *window);
    std::vector<CEGraphicObject *> *getObjects();
    bool renderToCanvas();
    uint64_t render();
    uint64_t loop();
    uint64_t loopAndRender();

private:
    CECanvas *canvas;
    std::vector<CEGraphicObject *> objects;
    std::vector<CEGraphicObject *> toAddObjects;
    std::vector<CERenderWindow *> alteredWindows;
    std::vector<CEGraphicObject *> toDeleteObjects;
    void deleteMarkedObjects();
    void addPendingObjects();
};

#endif
