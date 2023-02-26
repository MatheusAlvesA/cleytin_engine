#include "ce_container.h"


CEContainer::CEContainer() {
   this->width = 0;
   this->height = 0;
   this->positioningStyle = CEPositioningStyle::RELATIVE;
   this->alignX = CEAlign::START;
   this->alignY = CEAlign::START;
   this->objects = new std::vector <CEGraphicObject*>();
}

CEContainer::~CEContainer() {
    for (size_t i = 0; i < this->objects->size(); i++) {
        delete this->objects->at(i);
    }
    delete this->objects;
}

// TODO Adicionar suporte para os métodos setRotation, setMirrored e setNegative
void CEContainer::setRotation(uint16_t rotation) {
   this->rotation = 0;
}

void CEContainer::setMirrored(bool mirrored) {
    this->mirrored = false;
}

void CEContainer::setNegative(bool negative) {
    this->negative = false;
}


unsigned int CEContainer::addObject(CEGraphicObject *obj) {
    this->objects->push_back(obj);
    std::sort(this->objects->begin(), this->objects->end(), compareObjectPriority);
    return (unsigned int) this->objects->size();
}

bool CEContainer::removeObject(CEGraphicObject *obj) {
    return this->removeObjectAt(this->getObjectIndex(obj));
}

bool CEContainer::removeObjectAt(size_t index) {
    if(index >= this->objects->size()) {
        return false;
    }

    while(index < this->objects->size()-1) {
        (*this->objects)[index] = (*this->objects)[index+1];
        index++;
    }
    this->objects->pop_back();
    return true;
}

size_t CEContainer::getObjectsCount() {
    return this->objects->size();
}

CEGraphicObject* CEContainer::getObjectAt(size_t index) {
    if(index >= this->objects->size()) {
        return NULL;
    }

    return (*this->objects)[index];
}

size_t CEContainer::getObjectIndex(CEGraphicObject* obj) {
    for (size_t i = 0; i < this->objects->size(); i++)
    {
        if(obj == (*this->objects)[i]) {
            return i;
        }
    }
    return this->objects->size()+1;
}

void CEContainer::setWidth(unsigned int w) {
    this->width = w;
}

void CEContainer::setHeight(unsigned int h) {
    this->height = h;
}

unsigned int CEContainer::getWidth() {
    return this->width;
}

unsigned int CEContainer::getHeight() {
    return this->height;
}

void CEContainer::setPositioningStyle(CEPositioningStyle style) {
    this->positioningStyle = style;
}

void CEContainer::setAlignX(CEAlign align) {
    this->alignX = align;
}

void CEContainer::setAlignY(CEAlign align) {
    this->alignY = align;
}

CERenderWindow* CEContainer::getRenderWindow() {
    CERenderWindow *window = this->getDefaultRenderWindow();
    CEPoint *start = new CEPoint((int) this->posX, (int) this->posY);
    CEPoint *end = new CEPoint((int) (this->posX + this->width), (int) (this->posY + this->height));
    window->setPoints(start, end);
    delete start;
    delete end;
    return window;
}

size_t CEContainer::getHeightSpaceLeft() {
    size_t occupied = 0;
    for (size_t i = 0; i < this->objects->size(); i++)
    {
        if(this->positioningStyle == CEPositioningStyle::FLEX_COLUMN) {
            occupied += this->objects->at(i)->getRenderWindowHeight();
        } else {
            size_t h = this->objects->at(i)->getRenderWindowHeight();
            if(h > occupied) {
                occupied = h;
            }
        }
    }
    int spaceLeft = ((int)this->height) - ((int)occupied);
    if(spaceLeft < 0) {
        return 0;
    }
    return (size_t) spaceLeft;
}

size_t CEContainer::getWidthSpaceLeft() {
    size_t occupied = 0;
    for (size_t i = 0; i < this->objects->size(); i++)
    {
        if(this->positioningStyle == CEPositioningStyle::FLEX_ROW) {
            occupied += this->objects->at(i)->getRenderWindowWidth();
        } else {
            size_t w = this->objects->at(i)->getRenderWindowWidth();
            if(w > occupied) {
                occupied = w;
            }
        }
    }
    int spaceLeft = ((int)this->width) - ((int)occupied);
    if(spaceLeft < 0) {
        return 0;
    }
    return (size_t) spaceLeft;
}

void CEContainer::positionObject(CEGraphicObject *obj, CERenderWindow *w, size_t spaceLeftX, size_t spaceLeftY, uint &offsetX, uint &offsetY) {
    if(this->positioningStyle == CEPositioningStyle::RELATIVE) {
        obj->setPos(obj->getPosX() + w->topLeft->x, obj->getPosY() + w->topLeft->y);
        return;
    }
    if(this->positioningStyle == CEPositioningStyle::FLEX_COLUMN) {
        this->positionObjectCol(obj, w, spaceLeftY, offsetY);
    } else {
        this->positionObjectRow(obj, w, spaceLeftX, offsetX);
    }
}

void CEContainer::positionObjectCol(CEGraphicObject *obj, CERenderWindow *w, size_t spaceLeftY, uint &offsetY) {
    switch (this->alignY)
    {
        case CEAlign::START:
            obj->setPosY(offsetY);
            offsetY += obj->getRenderWindowHeight();
            break;
        case CEAlign::END:
            if(offsetY == w->topLeft->y) {
                offsetY += spaceLeftY;
            }
            obj->setPosY(offsetY);
            offsetY += obj->getRenderWindowHeight();
            break;
        case CEAlign::CENTER:
            if(offsetY == w->topLeft->y) {
                offsetY += (spaceLeftY / 2);
            }
            obj->setPosY(offsetY);
            offsetY += obj->getRenderWindowHeight();
            break;
        case CEAlign::SPACE_BETWEEN:
            if(this->objects->size() <= 1) {
                obj->setPosY(offsetY + spaceLeftY / 2);
                offsetY += obj->getRenderWindowHeight() + (spaceLeftY / 2);
            } else {
                obj->setPosY(offsetY);
                offsetY += obj->getRenderWindowHeight() + (spaceLeftY / (this->objects->size()-1));
            }
            break;
        case CEAlign::SPACE_AROUND:
            if(this->objects->size() <= 1) {
                obj->setPosY(offsetY + spaceLeftY / 2);
                offsetY += obj->getRenderWindowHeight() + (spaceLeftY / 2);
            } else {
                offsetY += (spaceLeftY / (this->objects->size()+1));
                obj->setPosY(offsetY);
                offsetY += obj->getRenderWindowHeight();
            }
            break;
    }

    int x = 0;
    int widthLeft = 0;
    switch (this->alignX)
    {
        case CEAlign::START:
            obj->setPosX(w->topLeft->x);
            break;
        case CEAlign::END:
            x = ((int)obj->getMaxX()) - ((int)obj->getRenderWindowWidth());
            if(x < w->topLeft->x) {
                obj->setPosX(w->topLeft->x);
            } else {
                obj->setPosX(x);
            }
            break;
        case CEAlign::CENTER:
        case CEAlign::SPACE_BETWEEN:
        case CEAlign::SPACE_AROUND:
            widthLeft = ((int)w->getWidth() - (int)obj->getRenderWindowWidth());
            if(widthLeft <= 0) {
                obj->setPosX(w->topLeft->x);
            } else {
                obj->setPosX(w->topLeft->x + (widthLeft/2));
            }
            break;
    }
}

void CEContainer::positionObjectRow(CEGraphicObject *obj, CERenderWindow *w, size_t spaceLeftX, uint &offsetX) {
    switch (this->alignX)
    {
        case CEAlign::START:
            obj->setPosX(offsetX);
            offsetX += obj->getRenderWindowWidth();
            break;
        case CEAlign::END:
            if(offsetX == w->topLeft->x) {
                offsetX += spaceLeftX;
            }
            obj->setPosX(offsetX);
            offsetX += obj->getRenderWindowWidth();
            break;
        case CEAlign::CENTER:
            if(offsetX == w->topLeft->x) {
                offsetX += (spaceLeftX / 2);
            }
            obj->setPosX(offsetX);
            offsetX += obj->getRenderWindowWidth();
            break;
        case CEAlign::SPACE_BETWEEN:
            if(this->objects->size() <= 1) {
                obj->setPosX(offsetX + spaceLeftX / 2);
                offsetX += obj->getRenderWindowWidth() + (spaceLeftX / 2);
            } else {
                obj->setPosX(offsetX);
                offsetX += obj->getRenderWindowWidth() + (spaceLeftX / (this->objects->size()-1));
            }
            break;
        case CEAlign::SPACE_AROUND:
            if(this->objects->size() <= 1) {
                obj->setPosX(offsetX + spaceLeftX / 2);
                offsetX += obj->getRenderWindowWidth() + (spaceLeftX / 2);
            } else {
                offsetX += (spaceLeftX / (this->objects->size()+1));
                obj->setPosX(offsetX);
                offsetX += obj->getRenderWindowWidth();
            }
            break;
    }

    int y = 0;
    int heightLeft = 0;
    switch (this->alignY)
    {
        case CEAlign::START:
            obj->setPosY(w->topLeft->y);
            break;
        case CEAlign::END:
            y = ((int)obj->getMaxY()) - ((int)obj->getRenderWindowHeight());
            if(y < w->topLeft->y) {
                obj->setPosY(w->topLeft->y);
            } else {
                obj->setPosY(y);
            }
            break;
        case CEAlign::CENTER:
        case CEAlign::SPACE_BETWEEN:
        case CEAlign::SPACE_AROUND:
            heightLeft = ((int)w->getHeight() - (int)obj->getRenderWindowHeight());
            if(heightLeft <= 0) {
                obj->setPosY(w->topLeft->y);
            } else {
                obj->setPosY(w->topLeft->y + (heightLeft/2));
            }
            break;
    }
}

bool CEContainer::renderToCanvas(CECanvas *canvas, CERenderWindow *window) {
    bool r = true;
    unsigned int bkpX = 0;
    unsigned int bkpY = 0;
    uint maxX = window->bottomRight->x;
    uint maxY = window->bottomRight->y;
    uint offsetX = window->topLeft->x;
    uint offsetY = window->topLeft->y;
    size_t spaceYLeft = this->getHeightSpaceLeft();
    size_t spaceXLeft = this->getWidthSpaceLeft();
    for (size_t i = 0; i < this->objects->size() && offsetY <= maxY && offsetX <= maxX; i++) {
        CEGraphicObject *obj = this->objects->at(i);
        bkpX = obj->getPosX();
        bkpY = obj->getPosY();
        obj->setMaxX(maxX);
        obj->setMaxY(maxY);
        this->positionObject(obj, window, spaceXLeft, spaceYLeft, offsetX, offsetY);
        if(!obj->renderToCanvas(canvas)) {
            r = false;
        }
        obj->setPos(bkpX, bkpY);
    }
    return r;
}
