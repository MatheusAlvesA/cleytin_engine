#ifndef CLEYTIN_ENGINE_H
#define CLEYTIN_ENGINE_H

#include <vector>
#include <algorithm>
#include "esp_system.h"
#include "ce_canvas_tftlcd_320x240.h"

#include <math.h>

#define PI 3.142857

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

    bool operator==(const CERenderWindow &window);
    bool operator!=(const CERenderWindow &window);

private:
    unsigned int maxX;
    unsigned int maxY;
};

class CEGraphicObject
{
public:
    CEGraphicObject();
    virtual ~CEGraphicObject();
    virtual CERenderWindow *getDefaultRenderWindow();
    virtual CERenderWindow *getRenderWindow() = 0;
    virtual bool renderToCanvas(CECanvas *canvas);
    virtual bool renderToCanvas(CECanvas *canvas, CERenderWindow *window) = 0;
    virtual bool containsPoint(CEPoint *point, unsigned int expand = 0);
    virtual bool containsAnyPointsFrom(std::vector<CEPoint *> *points, const unsigned int expand = 0);
    virtual std::vector<CEPoint *> *getAllRenderWindowPoints();
    // Setters
    virtual void setVisible(bool visible);
    virtual void setMirrored(bool mirrored);
    virtual void setNegative(bool negative);
    virtual void setColisionEnabled(bool enabled);
    virtual void setPriority(unsigned int priority);
    virtual void setPosX(unsigned int x);
    virtual void setPosY(unsigned int y);
    virtual void setMaxX(unsigned int x);
    virtual void setMaxY(unsigned int y);
    virtual void setPos(unsigned int x, unsigned int y);
    virtual void setRotation(uint16_t rotation);
    virtual void setBaseColor(const CEColor color);
    // Getters
    virtual bool getVisible();
    virtual bool getMirrored();
    virtual bool getNegative();
    virtual bool getColisionEnabled();
    virtual unsigned int getPriority();
    virtual unsigned int getPosX();
    virtual unsigned int getPosY();
    virtual unsigned int getMaxX();
    virtual unsigned int getMaxY();
    virtual CEColor getBaseColor();
    virtual uint16_t getRotation();
    virtual size_t getRenderWindowHeight();
    virtual size_t getRenderWindowWidth();
    virtual CERenderWindow *getContainingWindow();
    virtual std::vector<CERenderWindow *> *getAlteredWindows();
    virtual void clearAlteredWindows();

protected:
    bool visible;
    bool colisionEnabled;
    bool mirrored;
    bool negative;
    unsigned int priority;
    unsigned int posX;
    unsigned int posY;
    unsigned int maxX;
    unsigned int maxY;
    uint16_t rotation;
    CEColor baseColor;
    std::vector<CERenderWindow *> *alteredWindows;

    bool setPixel(CECanvas *canvas, int x, int y, CEColor color);
    bool rotatePixel(int &x, int &y, uint16_t rot);
    void mirrorPixel(int &x);
    void addCurrentWindowAsAltered();
    bool addAlteredWindow(CERenderWindow *w);
};

class CleytinEngine
{
public:
    CleytinEngine();
    ~CleytinEngine();

    unsigned int addObject(CEGraphicObject *obj);
    bool removeObject(CEGraphicObject *obj, bool freeMemory = false);
    bool removeObjectAt(size_t index, bool freeMemory = false);
    void clear(bool freeMemory = false);
    std::vector<size_t> *getCollisionsOn(size_t index);
    CEGraphicObject *getObjectAt(size_t index);
    std::vector<size_t> *getObjectsAt(CEPoint *point);
    size_t getObjectIndex(CEGraphicObject *obj);
    size_t getObjectsCount();
    std::vector<CEGraphicObject *> *getObjectsOnWindow(CERenderWindow *window);
    bool renderToCanvas();
    uint64_t render();
    uint64_t loop();

private:
    CECanvas *canvas;
    std::vector<CEGraphicObject *> objects;
    std::vector<CERenderWindow *> alteredWindows;
};

class CEActiveObject : public CEGraphicObject
{
public:
    CEActiveObject();
    virtual ~CEActiveObject();
    virtual CERenderWindow *getDefaultRenderWindow();
    virtual CERenderWindow *getRenderWindow();
    virtual bool renderToCanvas(CECanvas *canvas, CERenderWindow *window);
    virtual bool containsPoint(CEPoint *point, unsigned int expand = 0);
    virtual bool containsAnyPointsFrom(std::vector<CEPoint *> *points, const unsigned int expand = 0);
    virtual std::vector<CEPoint *> *getAllRenderWindowPoints();
    // Setters
    virtual void setVisible(bool visible);
    virtual void setMirrored(bool mirrored);
    virtual void setNegative(bool negative);
    virtual void setColisionEnabled(bool enabled);
    virtual void setPriority(unsigned int priority);
    virtual void setPosX(unsigned int x);
    virtual void setPosY(unsigned int y);
    virtual void setMaxX(unsigned int x);
    virtual void setMaxY(unsigned int y);
    virtual void setPos(unsigned int x, unsigned int y);
    virtual void setRotation(uint16_t rotation);
    virtual void setBaseColor(const CEColor color);
    // Getters
    virtual bool getVisible();
    virtual bool getMirrored();
    virtual bool getNegative();
    virtual bool getColisionEnabled();
    virtual unsigned int getPriority();
    virtual unsigned int getPosX();
    virtual unsigned int getPosY();
    virtual unsigned int getMaxX();
    virtual unsigned int getMaxY();
    virtual CEColor getBaseColor();
    virtual uint16_t getRotation();
    virtual size_t getRenderWindowHeight();
    virtual size_t getRenderWindowWidth();
    virtual std::vector<CERenderWindow *> *getAlteredWindows();

    virtual void setup(CleytinEngine *engine) = 0;
    virtual void beforeLoop(CleytinEngine *engine) = 0;
    virtual void loop(CleytinEngine *engine) = 0;
    virtual void beforeRender(CleytinEngine *engine) = 0;

protected:
    CEGraphicObject *graphicObject;
    std::vector<CERenderWindow *> *emptyWindows;
};

#endif
