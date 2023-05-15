#include "cleytin_engine_utils.h"

bool compareObjectPriority(CEGraphicObject *a, CEGraphicObject *b)
{
    return a->getPriority() < b->getPriority();
}

std::vector<CERenderWindow *> *remove_sub_container_windows(std::vector<CERenderWindow *> *list) {
    std::vector<CERenderWindow *> *r = new std::vector<CERenderWindow *>();

    if(list->size() <= 0) return r;

    for (size_t i = 0; i < list->size(); i++) {
        CERenderWindow *candidate = list->at(i)->clone();
        // Testando se alguma janela contém a janela candidata
        bool contained = false;
        for (size_t j = 0; j < list->size(); j++)
        {
            if(j == i) continue;
            if(list->at(j)->containsWindow(candidate)) {
                delete candidate;
                contained = true;
                break;
            }
        }
        if(contained) continue;
        // Testando se alguma janela já adicionada contém a janela candidata
        for (size_t j = 0; j < r->size(); j++)
        {
            if(r->at(j)->containsWindow(candidate)) {
                delete candidate;
                contained = true;
                break;
            }
        }
        if(contained) continue;

        r->push_back(candidate);
    }

    // Se não houver nenhuma janela, então todas as janelas são sub-janelas
    // umas das outras, então adicione apenas a primeira janela da lista
    if(r->size() <= 0) {
        r->push_back(list->at(0)->clone());
    }

    return r;
}

CERenderWindow *fuse_container_from_top(CERenderWindow *container, CERenderWindow *candidate) {
    if(
        *container->topLeft != *candidate->bottomLeft ||
        *container->topRight != *candidate->bottomRight
    ) {
        // Não é possível fundir as janelas
        return NULL;
    }

    CERenderWindow *r = new CERenderWindow(candidate->topLeft, container->bottomRight);
    return r;
}

CERenderWindow *fuse_container_from_bottom(CERenderWindow *container, CERenderWindow *candidate) {
    if(
        *container->bottomLeft != *candidate->topLeft ||
        *container->bottomRight != *candidate->topRight
    ) {
        // Não é possível fundir as janelas
        return NULL;
    }

    CERenderWindow *r = new CERenderWindow(container->topLeft, candidate->bottomRight);
    return r;
}

CERenderWindow *fuse_container_from_left(CERenderWindow *container, CERenderWindow *candidate) {
    if(
        *container->topLeft != *candidate->topRight ||
        *container->bottomLeft != *candidate->bottomRight
    ) {
        // Não é possível fundir as janelas
        return NULL;
    }

    CERenderWindow *r = new CERenderWindow(candidate->topLeft, container->bottomRight);
    return r;
}

CERenderWindow *fuse_container_from_right(CERenderWindow *container, CERenderWindow *candidate) {
    if(
        *container->topRight != *candidate->topLeft ||
        *container->bottomRight != *candidate->bottomLeft
    ) {
        // Não é possível fundir as janelas
        return NULL;
    }

    CERenderWindow *r = new CERenderWindow(container->topLeft, candidate->bottomRight);
    return r;
}

CERenderWindow *fuse_container(CERenderWindow *container, CERenderWindow *candidate) {
    CERenderWindow *fused = NULL;
    fused = fuse_container_from_top(container, candidate);
    if(fused != NULL) {
        return fused;
    }
    fused = fuse_container_from_bottom(container, candidate);
    if(fused != NULL) {
        return fused;
    }
    fused = fuse_container_from_left(container, candidate);
    if(fused != NULL) {
        return fused;
    }
    fused = fuse_container_from_right(container, candidate);
    if(fused != NULL) {
        return fused;
    }

    return NULL;
}

std::vector<CERenderWindow *> *fuse_container_windows(std::vector<CERenderWindow *> *list) {
    std::vector<CERenderWindow *> *r = new std::vector<CERenderWindow *>();

    if(list->size() <= 0) return r;

    std::vector<size_t> *processedIndexes = new std::vector<size_t>();

    for (size_t i = 0; i < list->size(); i++) {
        CERenderWindow *candidate = list->at(i)->clone();
        CERenderWindow *fused = NULL;
        processedIndexes->push_back(i);

        for (size_t j = 0; j < list->size(); j++)
        {
            if(in_array<size_t>(j, processedIndexes)) continue;

            fused = fuse_container(candidate, list->at(j));
            if(fused != NULL) {
                processedIndexes->push_back(j);
                break;
            }
        }

        if(fused == NULL) {
            r->push_back(candidate);
        } else {
            delete candidate;
            r->push_back(fused);
        }
    }

    delete processedIndexes;
    return r;
}

std::vector<CERenderWindow *> *optimize_container_windows(std::vector<CERenderWindow *> *list) {
    std::vector<CERenderWindow *> *stage1 = remove_sub_container_windows(list);
    std::vector<CERenderWindow *> *stage2 = fuse_container_windows(stage1);
    //TODO otimizar removendo interseções

    delete_pointers_vector<CERenderWindow>(stage1);
    return stage2;
}
