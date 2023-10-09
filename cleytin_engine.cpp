#include "cleytin_engine.h"
#include "cleytin_engine_utils.h"

CleytinEngine::CleytinEngine()
{
    this->canvas = new CECanvasTFTLCD320x240();
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
    this->toAddObjects.push_back(obj);
    return (unsigned int) (this->objects.size() + this->toAddObjects.size());
}

void CleytinEngine::addPendingObjects()
{
    if(this->toAddObjects.size() <= 0) return;
    std::vector<CEGraphicObject *> *tmp = new std::vector<CEGraphicObject *>();
    for (size_t i = 0; i < this->toAddObjects.size(); i++)
    {
        tmp->push_back(this->toAddObjects[i]);
    }
    this->toAddObjects.clear();
    for (size_t i = 0; i < tmp->size(); i++)
    {
        this->objects.push_back(tmp->at(i));
        tmp->at(i)->setup(this);
    }
    delete tmp;
    std::sort(this->objects.begin(), this->objects.end(), compareObjectPriority);
}

void CleytinEngine::deleteMarkedObjects() {
    for(int i = 0; i < this->toDeleteObjects.size(); i++) {
        this->removeObject(this->toDeleteObjects[i], true);
    }
    this->toDeleteObjects.clear();
}

void CleytinEngine::markToDelete(CEGraphicObject *obj)
{
    return this->toDeleteObjects.push_back(obj);
}

bool CleytinEngine::removeObject(CEGraphicObject *obj, bool freeMemory)
{
    return this->removeObjectAt(this->getObjectIndex(obj), freeMemory);
}

bool CleytinEngine::removeObjectAt(size_t index, bool freeMemory)
{
    if (index >= this->objects.size())
    {
        return false;
    }

    this->alteredWindows.push_back(this->objects[index]->getContainingWindow());

    this->objects[index]->beforeRemove(this);
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
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        this->objects[i]->beforeRemove(this);
        if (freeMemory) {
            delete this->objects[i];
        }
    }
    this->objects.clear();
    for (size_t i = 0; i < this->toAddObjects.size(); i++)
    {
        if (freeMemory) {
            delete this->toAddObjects[i];
        }
    }
    this->toAddObjects.clear();

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

    CERenderWindow *objWindow = object->getRenderWindow();
    objWindow->rotate(object->getRotation());
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        if (i == index || !this->objects[i]->getColisionEnabled())
        {
            continue;
        }
        CERenderWindow *candidateWindow = this->objects[i]->getRenderWindow();
        candidateWindow->rotate(this->objects[i]->getRotation());
        bool overlap = false;
        if(this->objects[i]->getRotation() != 0 || object->getRotation() != 0) {
            overlap = objWindow->doOverlapRotated(candidateWindow);
        } else {
            overlap = objWindow->doOverlapNotRotated(candidateWindow);
        }
        if (overlap)
        {
            r->push_back(i);
        }
        delete candidateWindow;
    }
    delete objWindow;

    return r;
}

std::vector<size_t> *CleytinEngine::getCollisionsOn(CEGraphicObject *obj)
{
    return this->getCollisionsOn(this->getObjectIndex(obj));
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
        CERenderWindow *objWindow = obj->getContainingWindow();
        if(window->doOverlapNotRotated(objWindow)) {
            r->push_back(obj);
        }
        delete objWindow;
    }
    return r;
}

std::vector<CEGraphicObject *> *CleytinEngine::getObjects() {
    std::vector<CEGraphicObject *> *r = new std::vector<CEGraphicObject *>();
    for(size_t i = 0; i < this->objects.size(); i++) {
        r->push_back(this->objects[i]);
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

        if(
            w->topLeft->x > this->canvas->getCanvasWidth() ||
            w->topLeft->y > this->canvas->getCanvasHeight() ||
            w->bottomRight->x < 0 ||
            w->bottomRight->y < 0
        ) {
            continue;
        }

        bool r = this->canvas->prepareWindow(
            w->topLeft->x,
            w->topLeft->y,
            w->bottomRight->x,
            w->bottomRight->y
        );
        if(!r) continue; // Janela inválida, pulando render

        std::vector<CEGraphicObject *> *objs = this->getObjectsOnWindow(w);
        for (size_t i = 0; i < objs->size(); i++)
        {
            objs->at(i)->renderToCanvas(this->canvas, w);
        }
        delete objs;
        this->canvas->render();
    }

    delete_pointers_vector<CERenderWindow>(optimizedAlteredWindows);
    return true;
}

uint64_t CleytinEngine::loopAndRender() {
    uint64_t start = esp_timer_get_time();
    this->loop();
    this->render();
    return esp_timer_get_time() - start;
}

uint64_t CleytinEngine::render()
{
    uint64_t start = esp_timer_get_time();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        this->objects[i]->beforeRender(this);
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
    this->addPendingObjects();
    this->deleteMarkedObjects();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        this->objects[i]->beforeLoop(this);
    }
    this->deleteMarkedObjects();
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        this->objects[i]->loop(this);
    }
    uint64_t end = esp_timer_get_time();
    return end - start;
}
