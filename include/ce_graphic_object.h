#ifndef CE_GRAPHIC_OBJECT_H
#define CE_GRAPHIC_OBJECT_H

#include "cleytin_engine.fwd.h"
#include "ce_canvas.h"
#include <vector>

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

    virtual void setup(CleytinEngine *engine);
    virtual void beforeLoop(CleytinEngine *engine);
    virtual void loop(CleytinEngine *engine);
    virtual void beforeRender(CleytinEngine *engine);

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
    bool setPixel(CECanvas *canvas, int x, int y, uint16_t color);
    bool rotatePixel(int &x, int &y, uint16_t rot);
    void mirrorPixel(int &x);
    void addCurrentWindowAsAltered();
    bool addAlteredWindow(CERenderWindow *w);
};

#endif
