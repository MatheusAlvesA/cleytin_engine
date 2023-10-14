#ifndef CE_GRAPHIC_OBJECT_H
#define CE_GRAPHIC_OBJECT_H

#include "cleytin_engine.fwd.h"
#include "ce_canvas.h"
#include <vector>
#include <stddef.h>
#include <functional>

class CEGraphicObject
{
public:
    CEGraphicObject();
    virtual ~CEGraphicObject();
    virtual CERenderWindow *getDefaultRenderWindow();
    virtual CERenderWindow *getRenderWindow() = 0;
    virtual void renderToCanvas(CECanvas *canvas, CERenderWindow *subWindow);
    virtual void renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow) = 0;
    virtual bool containsPoint(CEPoint *point, unsigned int expand = 0);
    virtual bool containsAnyPointsFrom(std::vector<CEPoint *> *points, const unsigned int expand = 0);
    // Setters
    virtual void setVisible(bool visible);
    virtual void setMirrored(bool mirrored);
    virtual void setNegative(bool negative);
    virtual void setColisionEnabled(bool enabled);
    virtual void setPriority(unsigned int priority);
    virtual void setPosX(int x);
    virtual void setPosY(int y);
    virtual void setMaxX(unsigned int x);
    virtual void setMaxY(unsigned int y);
    virtual void setPos(int x, int y);
    virtual void setBaseColor(const CEColor color);
    // Getters
    virtual bool getVisible();
    virtual bool getMirrored();
    virtual bool getNegative();
    virtual bool getColisionEnabled();
    virtual unsigned int getPriority();
    virtual int getPosX();
    virtual int getPosY();
    virtual unsigned int getMaxX();
    virtual unsigned int getMaxY();
    virtual CEColor getBaseColor();
    virtual size_t getRenderWindowHeight();
    virtual size_t getRenderWindowWidth();
    virtual std::vector<CERenderWindow *> *getAlteredWindows();
    virtual void clearAlteredWindows();

    virtual void setup(CleytinEngine *engine);
    virtual void beforeLoop(CleytinEngine *engine);
    virtual void loop(CleytinEngine *engine);
    virtual void beforeRender(CleytinEngine *engine);
    virtual void beforeRemove(CleytinEngine *engine);

protected:
    bool visible;
    bool colisionEnabled;
    bool mirrored;
    bool negative;
    unsigned int priority;
    int posX;
    int posY;
    unsigned int maxX;
    unsigned int maxY;
    CEColor baseColor;
    std::vector<CERenderWindow *> *alteredWindows;

    bool setPixel(CECanvas *canvas, int x, int y, CEColor color);
    bool setPixel(CECanvas *canvas, int x, int y, uint16_t color);
    void mirrorPixel(int &x);
    virtual void addCurrentWindowAsAltered();
    bool addAlteredWindow(CERenderWindow *w);
};

#endif
