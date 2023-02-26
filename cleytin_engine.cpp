#include "cleytin_engine.h"

bool compareObjectPriority(CEGraphicObject *a, CEGraphicObject *b) {
    return a->getPriority() < b->getPriority();
}

void delete_points_vector(std::vector<CEPoint *> *v) {
    for (size_t i = 0; i < v->size(); i++)
    {
        delete (*v)[i];
    }
    delete v;
}

const static float cosLookUp[360] = {1.0000, 0.9998, 0.9994, 0.9986, 0.9976, 0.9962, 0.9945, 0.9925, 0.9903, 0.9877, 0.9848, 0.9816, 0.9781, 0.9743, 0.9703, 0.9659, 0.9612, 0.9563, 0.9510, 0.9455, 0.9396, 0.9335, 0.9271, 0.9204, 0.9135, 0.9062, 0.8987, 0.8909, 0.8829, 0.8745, 0.8659, 0.8571, 0.8479, 0.8385, 0.8289, 0.8190, 0.8089, 0.7985, 0.7878, 0.7770, 0.7659, 0.7545, 0.7429, 0.7311, 0.7191, 0.7069, 0.6944, 0.6818, 0.6689, 0.6558, 0.6425, 0.6290, 0.6154, 0.6015, 0.5875, 0.5733, 0.5589, 0.5443, 0.5296, 0.5147, 0.4996, 0.4844, 0.4691, 0.4536, 0.4380, 0.4222, 0.4063, 0.3903, 0.3742, 0.3579, 0.3416, 0.3251, 0.3085, 0.2919, 0.2751, 0.2583, 0.2414, 0.2244, 0.2074, 0.1903, 0.1731, 0.1559, 0.1386, 0.1213, 0.1039, 0.0866, 0.0692, 0.0517, 0.0343, 0.0168, -0.0000, -0.0181, -0.0355, -0.0530, -0.0704, -0.0878, -0.1052, -0.1225, -0.1399, -0.1571, -0.1743, -0.1915, -0.2086, -0.2257, -0.2426, -0.2595, -0.2764, -0.2931, -0.3097, -0.3263, -0.3427, -0.3591, -0.3753, -0.3915, -0.4075, -0.4234, -0.4391, -0.4547, -0.4702, -0.4855, -0.5007, -0.5158, -0.5306, -0.5454, -0.5599, -0.5743, -0.5885, -0.6025, -0.6164, -0.6300, -0.6435, -0.6568, -0.6698, -0.6827, -0.6953, -0.7078, -0.7200, -0.7320, -0.7438, -0.7553, -0.7667, -0.7778, -0.7886, -0.7992, -0.8096, -0.8197, -0.8296, -0.8392, -0.8486, -0.8577, -0.8666, -0.8751, -0.8834, -0.8915, -0.8993, -0.9068, -0.9140, -0.9209, -0.9276, -0.9340, -0.9401, -0.9459, -0.9514, -0.9566, -0.9616, -0.9662, -0.9706, -0.9746, -0.9784, -0.9819, -0.9850, -0.9879, -0.9904, -0.9927, -0.9946, -0.9963, -0.9976, -0.9987, -0.9994, -0.9999, -1.0000, -0.9998, -0.9993, -0.9986, -0.9975, -0.9961, -0.9944, -0.9924, -0.9901, -0.9875, -0.9846, -0.9814, -0.9779, -0.9741, -0.9700, -0.9656, -0.9609, -0.9559, -0.9506, -0.9451, -0.9392, -0.9331, -0.9267, -0.9199, -0.9130, -0.9057, -0.8982, -0.8903, -0.8823, -0.8739, -0.8653, -0.8564, -0.8473, -0.8379, -0.8282, -0.8183, -0.8081, -0.7977, -0.7871, -0.7762, -0.7651, -0.7537, -0.7421, -0.7303, -0.7182, -0.7060, -0.6935, -0.6808, -0.6679, -0.6548, -0.6415, -0.6281, -0.6144, -0.6005, -0.5865, -0.5722, -0.5578, -0.5432, -0.5285, -0.5136, -0.4985, -0.4833, -0.4680, -0.4525, -0.4368, -0.4211, -0.4052, -0.3891, -0.3730, -0.3567, -0.3404, -0.3239, -0.3073, -0.2907, -0.2739, -0.2571, -0.2402, -0.2232, -0.2061, -0.1890, -0.1718, -0.1546, -0.1374, -0.1200, -0.1027, -0.0853, -0.0679, -0.0505, -0.0330, -0.0156, 0.0000, 0.0194, 0.0368, 0.0543, 0.0717, 0.0891, 0.1065, 0.1238, 0.1411, 0.1584, 0.1756, 0.1927, 0.2098, 0.2269, 0.2439, 0.2608, 0.2776, 0.2943, 0.3109, 0.3275, 0.3439, 0.3603, 0.3765, 0.3926, 0.4086, 0.4245, 0.4402, 0.4558, 0.4713, 0.4866, 0.5018, 0.5168, 0.5317, 0.5464, 0.5610, 0.5753, 0.5895, 0.6035, 0.6174, 0.6310, 0.6445, 0.6577, 0.6708, 0.6836, 0.6962, 0.7087, 0.7209, 0.7329, 0.7446, 0.7562, 0.7675, 0.7786, 0.7894, 0.8000, 0.8104, 0.8205, 0.8303, 0.8399, 0.8493, 0.8584, 0.8672, 0.8757, 0.8840, 0.8921, 0.8998, 0.9073, 0.9145, 0.9214, 0.9281, 0.9344, 0.9405, 0.9463, 0.9518, 0.9570, 0.9619, 0.9666, 0.9709, 0.9749, 0.9787, 0.9821, 0.9852, 0.9881, 0.9906, 0.9928, 0.9948, 0.9964, 0.9977, 0.9988, 0.9995, 0.9999};
const static float sinLookUp[360] = {0.0000, 0.0175, 0.0349, 0.0524, 0.0698, 0.0872, 0.1046, 0.1219, 0.1392, 0.1565, 0.1737, 0.1909, 0.2080, 0.2250, 0.2420, 0.2589, 0.2757, 0.2925, 0.3091, 0.3257, 0.3422, 0.3585, 0.3747, 0.3909, 0.4069, 0.4228, 0.4385, 0.4542, 0.4696, 0.4850, 0.5002, 0.5152, 0.5301, 0.5448, 0.5594, 0.5738, 0.5880, 0.6020, 0.6159, 0.6295, 0.6430, 0.6563, 0.6693, 0.6822, 0.6949, 0.7073, 0.7196, 0.7316, 0.7434, 0.7549, 0.7663, 0.7774, 0.7882, 0.7989, 0.8092, 0.8194, 0.8293, 0.8389, 0.8483, 0.8574, 0.8662, 0.8748, 0.8832, 0.8912, 0.8990, 0.9065, 0.9137, 0.9207, 0.9274, 0.9338, 0.9399, 0.9457, 0.9512, 0.9565, 0.9614, 0.9661, 0.9704, 0.9745, 0.9783, 0.9817, 0.9849, 0.9878, 0.9903, 0.9926, 0.9946, 0.9962, 0.9976, 0.9987, 0.9994, 0.9999, 1.0000, 0.9998, 0.9994, 0.9986, 0.9975, 0.9961, 0.9945, 0.9925, 0.9902, 0.9876, 0.9847, 0.9815, 0.9780, 0.9742, 0.9701, 0.9657, 0.9611, 0.9561, 0.9508, 0.9453, 0.9394, 0.9333, 0.9269, 0.9202, 0.9132, 0.9060, 0.8984, 0.8906, 0.8826, 0.8742, 0.8656, 0.8567, 0.8476, 0.8382, 0.8286, 0.8186, 0.8085, 0.7981, 0.7875, 0.7766, 0.7655, 0.7541, 0.7425, 0.7307, 0.7187, 0.7064, 0.6940, 0.6813, 0.6684, 0.6553, 0.6420, 0.6286, 0.6149, 0.6010, 0.5870, 0.5727, 0.5583, 0.5438, 0.5290, 0.5141, 0.4991, 0.4839, 0.4685, 0.4530, 0.4374, 0.4216, 0.4057, 0.3897, 0.3736, 0.3573, 0.3410, 0.3245, 0.3079, 0.2913, 0.2745, 0.2577, 0.2408, 0.2238, 0.2068, 0.1896, 0.1725, 0.1552, 0.1380, 0.1207, 0.1033, 0.0859, 0.0685, 0.0511, 0.0336, 0.0162,  0.0000, -0.0187, -0.0362, -0.0536, -0.0710, -0.0885, -0.1058, -0.1232, -0.1405, -0.1577, -0.1750, -0.1921, -0.2092, -0.2263, -0.2432, -0.2601, -0.2770, -0.2937, -0.3103, -0.3269, -0.3433, -0.3597, -0.3759, -0.3920, -0.4080, -0.4239, -0.4397, -0.4553, -0.4708, -0.4861, -0.5013, -0.5163, -0.5312, -0.5459, -0.5604, -0.5748, -0.5890, -0.6030, -0.6169, -0.6305, -0.6440, -0.6572, -0.6703, -0.6831, -0.6958, -0.7082, -0.7204, -0.7324, -0.7442, -0.7558, -0.7671, -0.7782, -0.7890, -0.7996, -0.8100, -0.8201, -0.8300, -0.8396, -0.8489, -0.8580, -0.8669, -0.8754, -0.8837, -0.8918, -0.8995, -0.9070, -0.9142, -0.9212, -0.9278, -0.9342, -0.9403, -0.9461, -0.9516, -0.9568, -0.9618, -0.9664, -0.9707, -0.9748, -0.9785, -0.9820, -0.9851, -0.9880, -0.9905, -0.9928, -0.9947, -0.9964, -0.9977, -0.9987, -0.9995, -0.9999, -1.0000, -0.9998, -0.9993, -0.9985, -0.9974, -0.9960, -0.9943, -0.9923, -0.9900, -0.9874, -0.9845, -0.9812, -0.9777, -0.9739, -0.9698, -0.9654, -0.9607, -0.9557, -0.9504, -0.9449, -0.9390, -0.9328, -0.9264, -0.9197, -0.9127, -0.9054, -0.8979, -0.8901, -0.8820, -0.8736, -0.8650, -0.8561, -0.8469, -0.8375, -0.8278, -0.8179, -0.8078, -0.7973, -0.7867, -0.7758, -0.7646, -0.7533, -0.7417, -0.7299, -0.7178, -0.7055, -0.6931, -0.6804, -0.6675, -0.6544, -0.6411, -0.6276, -0.6139, -0.6000, -0.5859, -0.5717, -0.5573, -0.5427, -0.5280, -0.5131, -0.4980, -0.4828, -0.4674, -0.4519, -0.4363, -0.4205, -0.4046, -0.3886, -0.3724, -0.3561, -0.3398, -0.3233, -0.3067, -0.2901, -0.2733, -0.2565, -0.2396, -0.2226, -0.2055, -0.1884, -0.1712, -0.1540, -0.1367, -0.1194, -0.1021, -0.0847, -0.0673, -0.0498, -0.0324, -0.0149};

/* CleytinEngine */

CleytinEngine::CleytinEngine()
{
    this->canvas = new CECanvasTFTLCD320x240();
    this->canvas->startRender();
    this->canvas->waitRenderFinish();
}

CleytinEngine::~CleytinEngine()
{
    delete this->canvas;
}

unsigned int CleytinEngine::addObject(CEGraphicObject *obj) {
    this->objects.push_back(obj);
    std::sort(this->objects.begin(), this->objects.end(), compareObjectPriority);
    CEActiveObject *aObj = dynamic_cast<CEActiveObject *>(obj);
    if(aObj) {
        aObj->setup(this);
    }
    return (unsigned int) this->objects.size();
}

bool CleytinEngine::removeObject(CEGraphicObject *obj, bool freeMemory) {
    size_t index = this->objects.size();
    for (size_t i = 0; i < this->objects.size(); i++) {
        if(this->objects[i] == obj) {
            index = i;
            break;
        }
    }

    return this->removeObjectAt(index, freeMemory);
}

bool CleytinEngine::removeObjectAt(size_t index, bool freeMemory) {
    if(index >= this->objects.size()) {
        return false;
    }

    if(freeMemory) {
        delete this->objects[index];
    }

    while(index < this->objects.size()-1) {
        this->objects[index] = this->objects[index+1];
        index++;
    }
    this->objects.pop_back();
    return true;
}

size_t CleytinEngine::getObjectIndex(CEGraphicObject* obj) {
    for (size_t i = 0; i < this->objects.size(); i++)
    {
        if(obj == this->objects[i]) {
            return i;
        }
    }
    return this->objects.size()+1;
}

void CleytinEngine::clear(bool freeMemory) {
    if(freeMemory) {
        for (size_t i = 0; i < this->objects.size(); i++)
        {
            delete this->objects[i];
        }
        
    }
    this->objects.clear();
}

std::vector<size_t>* CleytinEngine::getObjectsAt(CEPoint *point) {
    std::vector<size_t> *r = new std::vector<size_t>();
    for (size_t i = 0; i < this->objects.size(); i++) {
        if(this->objects[i]->containsPoint(point)) {
            r->push_back(i);
        }
    }
    return r;
}

std::vector<size_t>* CleytinEngine::getCollisionsOn(size_t index) {
    std::vector<size_t> *r = new std::vector<size_t>();
    if(index >= this->objects.size()) {
        return r;
    }
    CEGraphicObject *object = this->objects[index];
    if(!object->getColisionEnabled()) {
        return r;
    }

    std::vector<CEPoint *> *targetPoints = object->getAllRenderWindowPoints();
    for (size_t i = 0; i < this->objects.size(); i++) {
        if(i == index || !this->objects[i]->getColisionEnabled()) {
            continue;
        }
        std::vector<CEPoint *> *candidatePoints = this->objects[i]->getAllRenderWindowPoints();
        if(
            object->containsAnyPointsFrom(candidatePoints, 1) ||
            this->objects[i]->containsAnyPointsFrom(targetPoints, 1)
        ) {
            r->push_back(i);
        }
        delete_points_vector(candidatePoints);
    }
    delete_points_vector(targetPoints);

    return r;
}

size_t CleytinEngine::getObjectsCount() {
    return this->objects.size();
}

CEGraphicObject* CleytinEngine::getObjectAt(size_t index) {
    if(index >= this->objects.size()) {
        return NULL;
    }

    return this->objects[index];
}

void CleytinEngine::renderToCanvas() {
    this->canvas->clear();
    for (size_t i = 0; i < this->objects.size(); i++) {
        this->objects[i]->renderToCanvas(this->canvas);
    }
}

uint64_t CleytinEngine::render() {
    uint64_t start = esp_timer_get_time();
    this->canvas->waitRenderFinish(); // Finalize o render anterior caso não tenha
    for (size_t i = 0; i < this->objects.size(); i++) {
        CEActiveObject *obj = dynamic_cast<CEActiveObject *>(this->objects[i]);
        if(obj) {
            obj->beforeRender(this);
        }
    }
    this->renderToCanvas();
    this->canvas->startRender();
    return esp_timer_get_time() - start;
}

uint64_t CleytinEngine::waitRender() {
    uint64_t start = esp_timer_get_time();
    this->canvas->waitRenderFinish();
    return esp_timer_get_time() - start;
}

uint64_t CleytinEngine::renderSync() {
    uint64_t renderTime = this->render();
    uint64_t sendTime = this->waitRender();
    return renderTime + sendTime;
}


uint64_t CleytinEngine::loop() {
    uint64_t start = esp_timer_get_time();
    for (size_t i = 0; i < this->objects.size(); i++) {
        CEActiveObject *obj = dynamic_cast<CEActiveObject *>(this->objects[i]);
        if(obj) {
            obj->beforeLoop(this);
        }
    }
    for (size_t i = 0; i < this->objects.size(); i++) {
        CEActiveObject *obj = dynamic_cast<CEActiveObject *>(this->objects[i]);
        if(obj) {
            obj->loop(this);
        }
    }
    uint64_t end = esp_timer_get_time();
    return end - start;
}

/* CEPoint */

CEPoint::CEPoint(int x, int y) {
    this->x = x;
    this->y = y;
}

bool CEPoint::operator==(const CEPoint &dot) {
    return this->x == dot.x && this->y == dot.y;
}

void CEPoint::rotate(CEPoint *rotationCenter, uint16_t degrees) {
    if(degrees == 0) {
        return;
    }

    int normX = this->x - rotationCenter->x;
    int normY = this->y - rotationCenter->y;

    this->x = (normX * cosLookUp[degrees] - normY * sinLookUp[degrees]) + rotationCenter->x;
    this->y = (normX * sinLookUp[degrees] + normY * cosLookUp[degrees]) + rotationCenter->y;
}

CEPoint* CEPoint::clone() {
    return new CEPoint(this->x, this->y);
}

unsigned int CEPoint::distanceTo(const CEPoint point) {
    int dX = this->x - point.x;
    int dY = this->y - point.y;

    return (unsigned int) round(sqrt(dX*dX + dY*dY));
}

/* CELine */

CELine::CELine(const CEPoint &start, const CEPoint &end) {
    this->start = new CEPoint(start.x, start.y);
    this->end = new CEPoint(end.x, end.y);
}

CELine::~CELine() {
    delete this->start;
    delete this->end;
}

int CELine::calculateSideOfPoint(CEPoint* point) {
    return (point->y - this->start->y) * (this->end->x - this->start->x)
           - (point->x - this->start->x) * (this->end->y - this->start->y);
}


/* CERenderWindow */

CERenderWindow::CERenderWindow(const CEPoint *start, const CEPoint *end) {
    this->topLeft = NULL;
    this->bottomRight = NULL;
    this->topRight = NULL;
    this->bottomLeft = NULL;
    this->setPoints(start, end);
    this->maxX = LCD_WIDTH_PIXELS;
    this->maxY = LCD_HEIGHT_PIXELS;
}

void CERenderWindow::setMaxX(unsigned int x) {
    this->maxX = x;
}

void CERenderWindow::setMaxY(unsigned int y) {
    this->maxY = y;
}

void CERenderWindow::setPoints(const CEPoint *start, const CEPoint *end) {
    if(this->topLeft) {
        delete this->topLeft;    
    }
    this->topLeft = new CEPoint(start->x, start->y);

    if(this->bottomRight) {
        delete this->bottomRight;
    }
    this->bottomRight = new CEPoint(end->x, end->y);

    if(this->topRight) {
        delete this->topRight;
    }
    this->topRight = new CEPoint(end->x, start->y);

    if(this->bottomLeft) {
        delete this->bottomLeft;
    }
    this->bottomLeft = new CEPoint(start->x, end->y);
}

CERenderWindow::~CERenderWindow() {
    delete this->topLeft;
    delete this->topRight;
    delete this->bottomLeft;
    delete this->bottomRight;
}

CEPoint* CERenderWindow::getCenterPoint() {
    return new CEPoint(
        ((this->bottomRight->x - this->topLeft->x) / 2) + this->topLeft->x,
        ((this->bottomRight->y - this->topLeft->y) / 2) + this->topLeft->y
    );
}

CELine* CERenderWindow::getTopLine() {
    CEPoint *start = new CEPoint(this->topRight->x, this->topRight->y);
    CEPoint *end = new CEPoint(this->topLeft->x, this->topLeft->y);
    CELine *r = new CELine(*start, *end);
    delete start;
    delete end;
    return r;
}

CELine* CERenderWindow::getBottomLine() {
    CEPoint *start = new CEPoint(this->bottomLeft->x, this->bottomLeft->y);
    CEPoint *end = new CEPoint(this->bottomRight->x, this->bottomRight->y);
    CELine *r = new CELine(*start, *end);
    delete start;
    delete end;
    return r;
}

CELine* CERenderWindow::getLeftLine() {
    CEPoint *start = new CEPoint(this->topLeft->x, this->topLeft->y);
    CEPoint *end = new CEPoint(this->bottomLeft->x, this->bottomLeft->y);
    CELine *r = new CELine(*start, *end);
    delete start;
    delete end;
    return r;
}

CELine* CERenderWindow::getRightLine() {
    CEPoint *start = new CEPoint(this->bottomRight->x, this->bottomRight->y);
    CEPoint *end = new CEPoint(this->topRight->x, this->topRight->y);
    CELine *r = new CELine(*start, *end);
    delete start;
    delete end;
    return r;
}

void CERenderWindow::rotate(uint16_t degrees) {
    if(degrees == 0) {
        return;
    }

    CEPoint *center = this->getCenterPoint();

    this->topLeft->rotate(center, degrees);
    this->topRight->rotate(center, degrees);
    this->bottomLeft->rotate(center, degrees);
    this->bottomRight->rotate(center, degrees);
    delete center;
}

bool CERenderWindow::containsPoint(CEPoint *point) {
    CELine *topLine = this->getTopLine();
    CELine *bottomLine = this->getBottomLine();
    CELine *leftLine = this->getLeftLine();
    CELine *rightLine = this->getRightLine();

    int relativeTop = topLine->calculateSideOfPoint(point);
    int relativeBottom = bottomLine->calculateSideOfPoint(point);
    int relativeLeft = leftLine->calculateSideOfPoint(point);
    int relativeRight = rightLine->calculateSideOfPoint(point);

    bool r = false;
    if(
        (
            relativeTop >= 0 &&
            relativeBottom >= 0 &&
            relativeLeft >= 0 &&
            relativeRight >= 0
        ) ||
        (
            relativeTop <= 0 &&
            relativeBottom <= 0 &&
            relativeLeft <= 0 &&
            relativeRight <= 0
        )
    ) {
        r = true;
    }

    delete topLine;
    delete bottomLine;
    delete leftLine;
    delete rightLine;

    return r;
}

void CERenderWindow::expand(unsigned int size) {
    if(size >= this->maxY || size >= this->maxX) {
        return;
    }
    this->topLeft->x -= this->topLeft->x >= size ? size : 0;
    this->topLeft->y -= this->topLeft->y >= size ? size : 0;
    this->topRight->x += this->topRight->x < (this->maxX-size) ? size : 0;
    this->topRight->y -= this->topRight->y >= size ? size : 0;
    this->bottomLeft->x -= this->bottomLeft->x >= size ? size : 0;
    this->bottomLeft->y += this->bottomLeft->y < (this->maxY-size) ? size : 0;
    this->bottomRight->x += this->bottomRight->x < (this->maxX-size) ? size : 0;
    this->bottomRight->y += this->bottomRight->y < (this->maxY-size) ? size : 0;
}

std::vector<CEPoint*>* CERenderWindow::getAllPoints() {
    std::vector<CEPoint *> *points = new std::vector<CEPoint *>();
    points->push_back(this->topLeft->clone());
    points->push_back(this->topRight->clone());
    points->push_back(this->bottomLeft->clone());
    points->push_back(this->bottomRight->clone());
    return points;
}

size_t CERenderWindow::getHeight() {
    std::vector<CEPoint*>* points = this->getAllPoints();
    int min = points->at(0)->y;
    int max = points->at(0)->y;
    for (size_t i = 0; i < points->size(); i++)
    {
        if(points->at(i)->y < min) {
            min = points->at(i)->y;
        }
        if(points->at(i)->y > max) {
            max = points->at(i)->y;
        }
    }
    delete_points_vector(points);
    return (size_t)(max-min);
}

size_t CERenderWindow::getWidth() {
    std::vector<CEPoint*>* points = this->getAllPoints();
    int min = points->at(0)->x;
    int max = points->at(0)->x;
    for (size_t i = 0; i < points->size(); i++)
    {
        if(points->at(i)->x < min) {
            min = points->at(i)->x;
        }
        if(points->at(i)->x > max) {
            max = points->at(i)->x;
        }
    }
    delete_points_vector(points);
    return (size_t)(max-min);
}


/* CEGraphicObject */

CEGraphicObject::CEGraphicObject() {
    this->colisionEnabled = true;
    this->visible = true;
    this->mirrored = false;
    this->negative = false;
    this->priority = 0;
    this->posX = 0;
    this->posY = 0;
    this->rotation = 0;
    this->maxX = LCD_WIDTH_PIXELS;
    this->maxY = LCD_HEIGHT_PIXELS;
    this->baseColor = {0, 0, 0};
}

CEGraphicObject::~CEGraphicObject() {/* EMPTY */}

void CEGraphicObject::setVisible(bool visible) {
    this->visible = visible;
}

void CEGraphicObject::setMirrored(bool mirrored) {
    this->mirrored = mirrored;
}

void CEGraphicObject::setNegative(bool negative) {
    this->negative = negative;
}

void CEGraphicObject::setColisionEnabled(bool enabled) {
    this->colisionEnabled = enabled;
}

void CEGraphicObject::setPriority(unsigned int priority) {
    this->priority = priority;
}

void CEGraphicObject::setPosX(unsigned int posX) {
    this->posX = posX;
}

void CEGraphicObject::setPosY(unsigned int posY) {
    this->posY = posY;
}

void CEGraphicObject::setMaxX(unsigned int maxX) {
    this->maxX = maxX;
}

void CEGraphicObject::setMaxY(unsigned int maxY) {
    this->maxY = maxY;
}

unsigned int CEGraphicObject::getMaxX() {
    return this->maxX;
}

unsigned int CEGraphicObject::getMaxY() {
    return this->maxY;
}

void CEGraphicObject::setRotation(uint16_t rotation) {
    this->rotation = rotation % 360;
}

void CEGraphicObject::setPos(unsigned int x, unsigned int y) {
    this->posX = x;
    this->posY = y;
}

void CEGraphicObject::setBaseColor(const CEColor color) {
    this->baseColor = color;
}

CEColor CEGraphicObject::getBaseColor() {
    return this->baseColor;
}

bool CEGraphicObject::renderToCanvas(CECanvas *canvas) {
    if(!this->getVisible()) {
        return true;
    }
    CERenderWindow *w = this->getRenderWindow();
    bool result = this->renderToCanvas(canvas, w);
    delete w;
    return result;
}

bool CEGraphicObject::getVisible() {
    return this->visible;
}

bool CEGraphicObject::getMirrored() {
    return this->mirrored;
}

bool CEGraphicObject::getNegative() {
    return this->negative;
}

bool CEGraphicObject::getColisionEnabled() {
    return this->colisionEnabled;
}

unsigned int CEGraphicObject::getPriority() {
    return this->priority;
}

unsigned int CEGraphicObject::getPosX() {
    return this->posX;
}

unsigned int CEGraphicObject::getPosY() {
    return this->posY;
}

uint16_t CEGraphicObject::getRotation() {
    return this->rotation;
}

size_t CEGraphicObject::getRenderWindowHeight() {
    CERenderWindow *w = this->getRenderWindow();
    w->rotate(this->getRotation());
    size_t size = w->getHeight();
    delete w;
    return size;
}

size_t CEGraphicObject::getRenderWindowWidth() {
    CERenderWindow *w = this->getRenderWindow();
    w->rotate(this->getRotation());
    size_t size = w->getWidth();
    delete w;
    return size;
}

void CEGraphicObject::mirrorPixel(int &x) {
    CERenderWindow *w = this->getRenderWindow();
    CEPoint *c = w->getCenterPoint();
    int centerX = c->x;
    delete w;
    delete c;

    x = (centerX - x) + centerX;
}

std::vector<CEPoint *> *CEGraphicObject::getAllRenderWindowPoints() {
    CERenderWindow *w = this->getRenderWindow();
    w->rotate(this->getRotation());
    std::vector<CEPoint *> *r = w->getAllPoints();
    delete w;

    return r;
}

bool CEGraphicObject::rotatePixel(int &x, int &y, uint16_t rot) {
    if(rot == 0) {
        if(x >= this->maxX || y >= this->maxY) {
            return false;
        }
        return true;
    }

    CERenderWindow *w = this->getRenderWindow();
    CEPoint *center = w->getCenterPoint();
    delete w;

    int normX = x - center->x;
    int normY = y - center->y;

    int newX = (normX * cosLookUp[rot] - normY * sinLookUp[rot]) + center->x;
    int newY = (normX * sinLookUp[rot] + normY * cosLookUp[rot]) + center->y;
    delete center;

    if(
        newX < 0 || newX >= this->maxX ||
        newY < 0 || newY >= this->maxY
    ) {
        return false;
    }

    x = newX;
    y = newY;

    return true;
}

bool CEGraphicObject::setPixel(CECanvas *canvas, int x, int y, CEColor color) {
    if(this->getMirrored()) {
        this->mirrorPixel(x);
    }
    if(!this->rotatePixel(x, y, this->getRotation())) {
        return false;
    }
    if(x < 0 || y < 0) {
        return false;
    }
    unsigned int posX = (unsigned int) x;
    unsigned int posY = (unsigned int) y;

    if(this->getNegative()) {
        color = -color;
    }

    canvas->setPixel(posX, posY, color);

    return true;
}

bool CEGraphicObject::containsPoint(CEPoint *point, unsigned int expand) {
    CERenderWindow *w = this->getRenderWindow();
    w->expand(expand);
    w->rotate(this->getRotation());
    bool r = w->containsPoint(point);
    delete w;
    return r;
}

bool CEGraphicObject::containsAnyPointsFrom(std::vector<CEPoint *> *points, const unsigned int expand) {
    for (size_t i = 0; i < points->size(); i++)
    {
        if(this->containsPoint((*points)[i], expand)) {
            return true;
        }
    }
    return false;
}

CERenderWindow* CEGraphicObject::getDefaultRenderWindow() {
    CEPoint *point = new CEPoint(this->posX, this->posY);
    CERenderWindow *window = new CERenderWindow(point, point);
    window->setMaxX(this->maxX);
    window->setMaxY(this->maxY);
    delete point;
    return window;
}


/* CEActiveObject */

CEActiveObject::CEActiveObject() {
    this->graphicObject = NULL;
}

CEActiveObject::~CEActiveObject() {
    if(this->graphicObject) {
        delete this->graphicObject;
    }
}

void CEActiveObject::setVisible(bool visible) {
    if(this->graphicObject) {
        this->graphicObject->setVisible(visible);
    }
}

void CEActiveObject::setMirrored(bool mirrored) {
    if(this->graphicObject) {
        this->graphicObject->setMirrored(mirrored);
    }
}

void CEActiveObject::setNegative(bool negative) {
    if(this->graphicObject) {
        this->graphicObject->setNegative(negative);
    }
}

void CEActiveObject::setColisionEnabled(bool enabled) {
    if(this->graphicObject) {
        this->graphicObject->setColisionEnabled(enabled);
    }
}

void CEActiveObject::setPriority(unsigned int priority) {
    if(this->graphicObject) {
        this->graphicObject->setPriority(priority);
    }
}

void CEActiveObject::setPosX(unsigned int posX) {
    if(this->graphicObject) {
        this->graphicObject->setPosX(posX);
    }
}

void CEActiveObject::setPosY(unsigned int posY) {
    if(this->graphicObject) {
        this->graphicObject->setPosY(posY);
    }
}

void CEActiveObject::setMaxX(unsigned int maxX) {
    if(this->graphicObject) {
        this->graphicObject->setMaxX(maxX);
    }
}

void CEActiveObject::setMaxY(unsigned int maxY) {
    if(this->graphicObject) {
        this->graphicObject->setMaxY(maxY);
    }
}

unsigned int CEActiveObject::getMaxX() {
    if(this->graphicObject) {
        return this->graphicObject->getMaxX();
    }
    return 0;
}

unsigned int CEActiveObject::getMaxY() {
    if(this->graphicObject) {
        return this->graphicObject->getMaxY();
    }
    return 0;
}

void CEActiveObject::setRotation(uint16_t rotation) {
    if(this->graphicObject) {
        this->graphicObject->setRotation(rotation);
    }
}

void CEActiveObject::setPos(unsigned int x, unsigned int y) {
    if(this->graphicObject) {
        this->graphicObject->setPos(x, y);
    }
}

void CEActiveObject::setBaseColor(const CEColor color) {
    if(this->graphicObject) {
        this->graphicObject->setBaseColor(color);
    }
}

CEColor CEActiveObject::getBaseColor() {
    if(this->graphicObject) {
        return this->graphicObject->getBaseColor();
    }
    return {0,0,0};
}

CERenderWindow* CEActiveObject::getRenderWindow() {
    if(this->graphicObject) {
        return this->graphicObject->getRenderWindow();
    }
    CEPoint *point = new CEPoint(0, 0);
    CERenderWindow *window = new CERenderWindow(point, point);
    delete point;
    return window;
}

bool CEActiveObject::renderToCanvas(CECanvas *canvas, CERenderWindow *window) {
    if(this->graphicObject) {
        return this->graphicObject->renderToCanvas(canvas, window);
    }
    return false;
}

bool CEActiveObject::getVisible() {
    if(this->graphicObject) {
        return this->graphicObject->getVisible();
    }
    return false;
}

bool CEActiveObject::getMirrored() {
    if(this->graphicObject) {
        return this->graphicObject->getMirrored();
    }
    return false;
}

bool CEActiveObject::getNegative() {
    if(this->graphicObject) {
        return this->graphicObject->getNegative();
    }
    return false;
}

bool CEActiveObject::getColisionEnabled() {
    if(this->graphicObject) {
        return this->graphicObject->getColisionEnabled();
    }
    return false;
}

unsigned int CEActiveObject::getPriority() {
    if(this->graphicObject) {
        return this->graphicObject->getPriority();
    }
    return 0;
}

unsigned int CEActiveObject::getPosX() {
    if(this->graphicObject) {
        return this->graphicObject->getPosX();
    }
    return 0;
}

unsigned int CEActiveObject::getPosY() {
    if(this->graphicObject) {
        return this->graphicObject->getPosY();
    }
    return 0;
}

uint16_t CEActiveObject::getRotation() {
    if(this->graphicObject) {
        return this->graphicObject->getRotation();
    }
    return 0;
}

size_t CEActiveObject::getRenderWindowHeight() {
    if(this->graphicObject) {
        return this->graphicObject->getRenderWindowHeight();
    }
    return 0;
}

size_t CEActiveObject::getRenderWindowWidth() {
    if(this->graphicObject) {
        return this->graphicObject->getRenderWindowWidth();
    }
    return 0;
}

std::vector<CEPoint *> *CEActiveObject::getAllRenderWindowPoints() {
    if(this->graphicObject) {
        return this->graphicObject->getAllRenderWindowPoints();
    }
    return new std::vector<CEPoint *>;
}

bool CEActiveObject::containsPoint(CEPoint *point, unsigned int expand) {
    if(this->graphicObject) {
        return this->graphicObject->containsPoint(point, expand);
    }
    return false;
}

bool CEActiveObject::containsAnyPointsFrom(std::vector<CEPoint *> *points, const unsigned int expand) {
    if(this->graphicObject) {
        return this->graphicObject->containsAnyPointsFrom(points, expand);
    }
    return false;
}

CERenderWindow* CEActiveObject::getDefaultRenderWindow() {
    if(this->graphicObject) {
        return this->graphicObject->getDefaultRenderWindow();
    }
    CEPoint *point = new CEPoint(0, 0);
    CERenderWindow *window = new CERenderWindow(point, point);
    window->setMaxX(0);
    window->setMaxY(0);
    return window;
}
