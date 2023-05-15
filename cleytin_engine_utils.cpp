#include "cleytin_engine.h"

bool compareObjectPriority(CEGraphicObject *a, CEGraphicObject *b)
{
    return a->getPriority() < b->getPriority();
}

template<typename T>
void delete_pointers_vector(std::vector<T *> *v)
{
    for (size_t i = 0; i < v->size(); i++)
    {
        delete (*v)[i];
    }
    delete v;
}

const static float cosLookUp[360] = {1.0000, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9743, 0.9703, 0.9659, 0.9612, 0.9563, 0.9510, 0.9455, 0.9396, 0.9335, 0.9271, 0.9204, 0.9135, 0.9062, 0.8987, 0.8909, 0.8829, 0.8745, 0.8659, 0.8571, 0.8479, 0.8385, 0.8289, 0.8190, 0.8089, 0.7985, 0.7878, 0.7770, 0.7659, 0.7545, 0.7429, 0.7311, 0.7191, 0.7069, 0.6944, 0.6818, 0.6689, 0.6558, 0.6425, 0.6290, 0.6154, 0.6015, 0.5875, 0.5733, 0.5589, 0.5443, 0.5296, 0.5147, 0.4996, 0.4844, 0.4691, 0.4536, 0.4380, 0.4222, 0.4063, 0.3903, 0.3742, 0.3579, 0.3416, 0.3251, 0.3085, 0.2919, 0.2751, 0.2583, 0.2414, 0.2244, 0.2074, 0.1903, 0.1731, 0.1559, 0.1386, 0.1213, 0.1039, 0.0866, 0.0692, 0.0517, 0.0343, 0.0168, -0.0000, -0.0181, -0.0355, -0.0530, -0.0704, -0.0878, -0.1052, -0.1225, -0.1399, -0.1571, -0.1743, -0.1915, -0.2086, -0.2257, -0.2426, -0.2595, -0.2764, -0.2931, -0.3097, -0.3263, -0.3427, -0.3591, -0.3753, -0.3915, -0.4075, -0.4234, -0.4391, -0.4547, -0.4702, -0.4855, -0.5007, -0.5158, -0.5306, -0.5454, -0.5599, -0.5743, -0.5885, -0.6025, -0.6164, -0.6300, -0.6435, -0.6568, -0.6698, -0.6827, -0.6953, -0.7078, -0.7200, -0.7320, -0.7438, -0.7553, -0.7667, -0.7778, -0.7886, -0.7992, -0.8096, -0.8197, -0.8296, -0.8392, -0.8486, -0.8577, -0.8666, -0.8751, -0.8834, -0.8915, -0.8993, -0.9068, -0.9140, -0.9209, -0.9276, -0.9340, -0.9401, -0.9459, -0.9514, -0.9566, -0.9616, -0.9662, -0.9706, -0.9746, -0.9784, -0.9819, -0.9850, -0.9879, -0.9904, -0.9927, -0.9946, -0.9963, -0.9976, -0.9987, -0.9994, -0.9999, -1.0000, -0.9998, -0.9993, -0.9986, -0.9975, -0.9961, -0.9944, -0.9924, -0.9901, -0.9875, -0.9846, -0.9814, -0.9779, -0.9741, -0.9700, -0.9656, -0.9609, -0.9559, -0.9506, -0.9451, -0.9392, -0.9331, -0.9267, -0.9199, -0.9130, -0.9057, -0.8982, -0.8903, -0.8823, -0.8739, -0.8653, -0.8564, -0.8473, -0.8379, -0.8282, -0.8183, -0.8081, -0.7977, -0.7871, -0.7762, -0.7651, -0.7537, -0.7421, -0.7303, -0.7182, -0.7060, -0.6935, -0.6808, -0.6679, -0.6548, -0.6415, -0.6281, -0.6144, -0.6005, -0.5865, -0.5722, -0.5578, -0.5432, -0.5285, -0.5136, -0.4985, -0.4833, -0.4680, -0.4525, -0.4368, -0.4211, -0.4052, -0.3891, -0.3730, -0.3567, -0.3404, -0.3239, -0.3073, -0.2907, -0.2739, -0.2571, -0.2402, -0.2232, -0.2061, -0.1890, -0.1718, -0.1546, -0.1374, -0.1200, -0.1027, -0.0853, -0.0679, -0.0505, -0.0330, -0.0156, 0.0000, 0.0194, 0.0368, 0.0543, 0.0717, 0.0891, 0.1065, 0.1238, 0.1411, 0.1584, 0.1756, 0.1927, 0.2098, 0.2269, 0.2439, 0.2608, 0.2776, 0.2943, 0.3109, 0.3275, 0.3439, 0.3603, 0.3765, 0.3926, 0.4086, 0.4245, 0.4402, 0.4558, 0.4713, 0.4866, 0.5018, 0.5168, 0.5317, 0.5464, 0.5610, 0.5753, 0.5895, 0.6035, 0.6174, 0.6310, 0.6445, 0.6577, 0.6708, 0.6836, 0.6962, 0.7087, 0.7209, 0.7329, 0.7446, 0.7562, 0.7675, 0.7786, 0.7894, 0.8000, 0.8104, 0.8205, 0.8303, 0.8399, 0.8493, 0.8584, 0.8672, 0.8757, 0.8840, 0.8921, 0.8998, 0.9073, 0.9145, 0.9214, 0.9281, 0.9344, 0.9405, 0.9463, 0.9518, 0.9570, 0.9619, 0.9666, 0.9709, 0.9749, 0.9787, 0.9821, 0.9852, 0.9881, 0.9906, 0.9928, 0.9948, 0.9964, 0.9977, 0.9988, 0.9995, 0.9999};
const static float sinLookUp[360] = {0.0000, 0.0175, 0.0349, 0.0524, 0.0698, 0.0872, 0.1046, 0.1219, 0.1392, 0.1565, 0.1737, 0.1909, 0.2080, 0.2250, 0.2420, 0.2589, 0.2757, 0.2925, 0.3091, 0.3257, 0.3422, 0.3585, 0.3747, 0.3909, 0.4069, 0.4228, 0.4385, 0.4542, 0.4696, 0.4850, 0.5002, 0.5152, 0.5301, 0.5448, 0.5594, 0.5738, 0.5880, 0.6020, 0.6159, 0.6295, 0.6430, 0.6563, 0.6693, 0.6822, 0.6949, 0.7073, 0.7196, 0.7316, 0.7434, 0.7549, 0.7663, 0.7774, 0.7882, 0.7989, 0.8092, 0.8194, 0.8293, 0.8389, 0.8483, 0.8574, 0.8662, 0.8748, 0.8832, 0.8912, 0.8990, 0.9065, 0.9137, 0.9207, 0.9274, 0.9338, 0.9399, 0.9457, 0.9512, 0.9565, 0.9614, 0.9661, 0.9704, 0.9745, 0.9783, 0.9817, 0.9849, 0.9878, 0.9903, 0.9926, 0.9946, 0.9962, 0.9976, 0.9987, 0.9994, 0.9999, 1.0000, 0.9998, 0.9994, 0.9986, 0.9975, 0.9961, 0.9945, 0.9925, 0.9902, 0.9876, 0.9847, 0.9815, 0.9780, 0.9742, 0.9701, 0.9657, 0.9611, 0.9561, 0.9508, 0.9453, 0.9394, 0.9333, 0.9269, 0.9202, 0.9132, 0.9060, 0.8984, 0.8906, 0.8826, 0.8742, 0.8656, 0.8567, 0.8476, 0.8382, 0.8286, 0.8186, 0.8085, 0.7981, 0.7875, 0.7766, 0.7655, 0.7541, 0.7425, 0.7307, 0.7187, 0.7064, 0.6940, 0.6813, 0.6684, 0.6553, 0.6420, 0.6286, 0.6149, 0.6010, 0.5870, 0.5727, 0.5583, 0.5438, 0.5290, 0.5141, 0.4991, 0.4839, 0.4685, 0.4530, 0.4374, 0.4216, 0.4057, 0.3897, 0.3736, 0.3573, 0.3410, 0.3245, 0.3079, 0.2913, 0.2745, 0.2577, 0.2408, 0.2238, 0.2068, 0.1896, 0.1725, 0.1552, 0.1380, 0.1207, 0.1033, 0.0859, 0.0685, 0.0511, 0.0336, 0.0162, 0.0000, -0.0187, -0.0362, -0.0536, -0.0710, -0.0885, -0.1058, -0.1232, -0.1405, -0.1577, -0.1750, -0.1921, -0.2092, -0.2263, -0.2432, -0.2601, -0.2770, -0.2937, -0.3103, -0.3269, -0.3433, -0.3597, -0.3759, -0.3920, -0.4080, -0.4239, -0.4397, -0.4553, -0.4708, -0.4861, -0.5013, -0.5163, -0.5312, -0.5459, -0.5604, -0.5748, -0.5890, -0.6030, -0.6169, -0.6305, -0.6440, -0.6572, -0.6703, -0.6831, -0.6958, -0.7082, -0.7204, -0.7324, -0.7442, -0.7558, -0.7671, -0.7782, -0.7890, -0.7996, -0.8100, -0.8201, -0.8300, -0.8396, -0.8489, -0.8580, -0.8669, -0.8754, -0.8837, -0.8918, -0.8995, -0.9070, -0.9142, -0.9212, -0.9278, -0.9342, -0.9403, -0.9461, -0.9516, -0.9568, -0.9618, -0.9664, -0.9707, -0.9748, -0.9785, -0.9820, -0.9851, -0.9880, -0.9905, -0.9928, -0.9947, -0.9964, -0.9977, -0.9987, -0.9995, -0.9999, -1.0000, -0.9998, -0.9993, -0.9985, -0.9974, -0.9960, -0.9943, -0.9923, -0.9900, -0.9874, -0.9845, -0.9812, -0.9777, -0.9739, -0.9698, -0.9654, -0.9607, -0.9557, -0.9504, -0.9449, -0.9390, -0.9328, -0.9264, -0.9197, -0.9127, -0.9054, -0.8979, -0.8901, -0.8820, -0.8736, -0.8650, -0.8561, -0.8469, -0.8375, -0.8278, -0.8179, -0.8078, -0.7973, -0.7867, -0.7758, -0.7646, -0.7533, -0.7417, -0.7299, -0.7178, -0.7055, -0.6931, -0.6804, -0.6675, -0.6544, -0.6411, -0.6276, -0.6139, -0.6000, -0.5859, -0.5717, -0.5573, -0.5427, -0.5280, -0.5131, -0.4980, -0.4828, -0.4674, -0.4519, -0.4363, -0.4205, -0.4046, -0.3886, -0.3724, -0.3561, -0.3398, -0.3233, -0.3067, -0.2901, -0.2733, -0.2565, -0.2396, -0.2226, -0.2055, -0.1884, -0.1712, -0.1540, -0.1367, -0.1194, -0.1021, -0.0847, -0.0673, -0.0498, -0.0324, -0.0149};

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

template<typename T>
bool in_array(T needle, std::vector<T> *haystack) {
    for (size_t i = 0; i < haystack->size(); i++)
    {
        if(haystack->at(i) == needle) return true;
    }
    return false;
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
