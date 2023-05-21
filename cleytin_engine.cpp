#include "cleytin_engine.h"
#include "cleytin_engine_utils.h"

CleytinEngine::CleytinEngine()
{
    this->canvas = new CECanvasTFTLCD320x240();
    this->canvas->render();
}

CleytinEngine::~CleytinEngine()
{
    delete this->canvas;
    for(int i = 0; i < this->alteredWindows.size(); i++) {
        delete this->alteredWindows[i];
    }
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

    this->alteredWindows.push_back(this->objects[index]->getContainingWindow()->clone());

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

    CEPoint *start = new CEPoint(0, 0);
    CEPoint *end = new CEPoint(this->canvas->getCanvasWidth(), this->canvas->getCanvasHeight());

    this->alteredWindows.push_back(new CERenderWindow(start, end));

    delete start;
    delete end;
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

std::vector<CEGraphicObject *> *CleytinEngine::getObjectsOnWindow(CERenderWindow *window) {
    std::vector<CEGraphicObject *> *r = new std::vector<CEGraphicObject *>();
    for(size_t i = 0; i < this->objects.size(); i++) {
        CEGraphicObject *obj = this->objects[i];
        std::vector<CEPoint *> *objPoints = obj->getAllRenderWindowPoints();
        for (size_t j = 0; j < objPoints->size(); j++)
        {
            if(window->containsPoint(objPoints->at(j))) {
                r->push_back(obj);
                break;
            }
        }
        
        delete_pointers_vector<CEPoint>(objPoints);
    }
    return r;
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

    for(size_t i = 0; i < this->alteredWindows.size(); i++) {
        alteredWindows->push_back(this->alteredWindows[i]->clone());
    }

    if(alteredWindows->size() <= 0) {
        delete alteredWindows;
        return false;
    }

    std::vector<CERenderWindow *> *optimizedAlteredWindows = optimize_container_windows(alteredWindows);
    delete_pointers_vector<CERenderWindow>(alteredWindows);

    for (size_t i = 0; i < optimizedAlteredWindows->size(); i++)
    {
        CERenderWindow *w = optimizedAlteredWindows->at(i);
        if(w->topLeft->x < 0) w->topLeft->x = 0;
        if(w->topLeft->y < 0) w->topLeft->y = 0;

        this->canvas->prepareWindow(
            w->topLeft->x,
            w->topLeft->y,
            w->bottomRight->x,
            w->bottomRight->y
        );

        std::vector<CEGraphicObject *> *objs = this->getObjectsOnWindow(w);
        for (size_t i = 0; i < objs->size(); i++)
        {
            objs->at(i)->renderToCanvas(this->canvas);
        }
        delete objs;
        this->canvas->render();
    }

    delete_pointers_vector<CERenderWindow>(optimizedAlteredWindows);
    return true;
}

uint64_t CleytinEngine::render()
{
    uint64_t start = esp_timer_get_time();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        CEActiveObject *obj = dynamic_cast<CEActiveObject *>(this->objects[i]);
        if (obj)
        {
            obj->beforeRender(this);
        }
    }
    
    this->renderToCanvas();

    for(int i = 0; i < this->alteredWindows.size(); i++) {
        delete this->alteredWindows[i];
    }
    this->alteredWindows.clear();

    return esp_timer_get_time() - start;
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
