#ifndef CE_CONTAINER_H
#define CE_CONTAINER_H

#include "cleytin_engine.h"

enum CEPositioningStyle {
    RELATIVE = 0,
    FLEX_COLUMN = 1,
    FLEX_ROW = 2,
};

enum CEAlign {
    START = 0,
    CENTER = 1,
    END = 2,
    SPACE_BETWEEN = 3,
    SPACE_AROUND = 4,
};

class CEContainer : public CEGraphicObject {
public:
    CEContainer();
    ~CEContainer();
    void setWidth(unsigned int w);
    void setHeight(unsigned int h);
    unsigned int getWidth();
    unsigned int getHeight();

    unsigned int addObject(CEGraphicObject *obj);
    bool removeObject(CEGraphicObject *obj);
    bool removeObjectAt(size_t index);
    CEGraphicObject* getObjectAt(size_t index);
    size_t getObjectIndex(CEGraphicObject* obj);
    size_t getObjectsCount();
    size_t getHeightSpaceLeft();
    size_t getWidthSpaceLeft();

    void setPositioningStyle(CEPositioningStyle style);
    void setAlignX(CEAlign align);
    void setAlignY(CEAlign align);

    void setRotation(uint16_t rotation);
    void setMirrored(bool mirrored);
    void setNegative(bool negative);

    CERenderWindow* getRenderWindow();
    bool renderToCanvas(CECanvas *canvas, CERenderWindow *window);

protected:
    unsigned int width;
    unsigned int height;
    CEPositioningStyle positioningStyle;
    CEAlign alignX;
    CEAlign alignY;
    std::vector <CEGraphicObject*> *objects;

    void positionObject(CEGraphicObject *obj, CERenderWindow *w, size_t spaceLeftX, size_t spaceLeftY, uint &offsetX, uint &offsetY);

private:
    void positionObjectRow(CEGraphicObject *obj, CERenderWindow *w, size_t spaceLeftX, uint &offsetX);
    void positionObjectCol(CEGraphicObject *obj, CERenderWindow *w, size_t spaceLeftY, uint &offsetY);
};

#endif
