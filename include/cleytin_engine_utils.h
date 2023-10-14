#ifndef CLEYTIN_ENGINE_UTILS_H
#define CLEYTIN_ENGINE_UTILS_H

#include "cleytin_engine.h"

bool compareObjectPriority(CEGraphicObject *a, CEGraphicObject *b);

template<typename T>
void delete_pointers_vector(std::vector<T *> *v)
{
    for (size_t i = 0; i < v->size(); i++)
    {
        delete (*v)[i];
    }
    delete v;
}

std::vector<CERenderWindow *> *remove_sub_container_windows(std::vector<CERenderWindow *> *list);

template<typename T>
bool in_array(T needle, std::vector<T> *haystack) {
    for (size_t i = 0; i < haystack->size(); i++)
    {
        if(haystack->at(i) == needle) return true;
    }
    return false;
}

CERenderWindow *fuse_container_dislocated(CERenderWindow *container, CERenderWindow *candidate);

CERenderWindow *fuse_container(CERenderWindow *container, CERenderWindow *candidate);

std::vector<CERenderWindow *> *fuse_container_windows(std::vector<CERenderWindow *> *list);

std::vector<CERenderWindow *> *optimize_container_windows(std::vector<CERenderWindow *> *list);

#endif
