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
        container->topLeft->x != candidate->bottomLeft->x ||
        container->topRight->x != candidate->bottomRight->x ||
        container->topLeft->y > candidate->bottomLeft->y ||
        container->topLeft->y < candidate->topLeft->y ||
        container->topRight->y > candidate->bottomRight->y ||
        container->topRight->y < candidate->topRight->y
    ) {
        // Não é possível fundir as janelas
        return NULL;
    }

    CERenderWindow *r = new CERenderWindow(candidate->topLeft, container->bottomRight);
    return r;
}

CERenderWindow *fuse_container_from_bottom(CERenderWindow *container, CERenderWindow *candidate) {
    if(
        container->bottomLeft->x != candidate->topLeft->x ||
        container->bottomRight->x != candidate->topRight->x ||
        container->bottomLeft->y < candidate->topLeft->y ||
        container->bottomLeft->y > candidate->bottomLeft->y ||
        container->bottomRight->y < candidate->topRight->y ||
        container->bottomRight->y > candidate->bottomRight->y
    ) {
        // Não é possível fundir as janelas
        return NULL;
    }

    CERenderWindow *r = new CERenderWindow(container->topLeft, candidate->bottomRight);
    return r;
}

CERenderWindow *fuse_container_from_left(CERenderWindow *container, CERenderWindow *candidate) {
    if(
        container->topLeft->y != candidate->topRight->y ||
        container->bottomLeft->y != candidate->bottomRight->y ||
        container->topLeft->x > candidate->topRight->x ||
        container->topLeft->x < candidate->topLeft->x ||
        container->bottomLeft->x > candidate->bottomRight->x ||
        container->bottomLeft->x < candidate->bottomLeft->x
    ) {
        // Não é possível fundir as janelas
        return NULL;
    }

    CERenderWindow *r = new CERenderWindow(candidate->topLeft, container->bottomRight);
    return r;
}

CERenderWindow *fuse_container_from_right(CERenderWindow *container, CERenderWindow *candidate) {
    if(
        container->topRight->y != candidate->topLeft->y ||
        container->bottomRight->y != candidate->bottomLeft->y ||
        container->topRight->x < candidate->topLeft->x ||
        container->topRight->x > candidate->topRight->x ||
        container->bottomRight->x < candidate->bottomLeft->x ||
        container->bottomRight->x > candidate->bottomRight->x
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
        if(in_array<size_t>(i, processedIndexes)) continue;

        CERenderWindow *fused = NULL;
        processedIndexes->push_back(i);

        for (size_t j = 0; j < list->size(); j++)
        {
            if(in_array<size_t>(j, processedIndexes)) continue;

            fused = fuse_container(list->at(i), list->at(j));
            if(fused != NULL) {
                processedIndexes->push_back(j);
                break;
            }
        }

        if(fused == NULL) {
            r->push_back(list->at(i)->clone());
        } else {
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
