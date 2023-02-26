#include "ce_menu_helper.h"


CEMenuHelper::CEMenuHelper() {
   this->width = LCD_WIDTH_PIXELS;
   this->height = LCD_HEIGHT_PIXELS;
   this->selected = 0;
   this->itemsOffset = 0;
   this->options = new std::vector <CEMenuOption>();
   this->ctrl = new CleytinControls();
   this->selectionMade = false;
   this->btnStartState = false;
   this->textColor = {0,0,0};
   this->highlightBGColor = {0,0,0};
   this->highlightTextColor = {0xFF,0xFF,0xFF};
   this->optionsSizeMultiplier = 2;
   this->title = NULL;
}

void CEMenuHelper::reset() {
   this->selected = 0;
   this->itemsOffset = 0;
   this->selectionMade = false;
   this->btnStartState = false;
}


CEMenuHelper::~CEMenuHelper() {
    for (size_t i = 0; i < this->options->size(); i++) {
        delete this->options->at(i).label;
    }
    delete this->options;
    delete this->ctrl;
    if(this->title != NULL) {
        delete this->title;
    }
}

void CEMenuHelper::setOptionsSizeMultiplier(uint8_t size) {
    this->optionsSizeMultiplier = size;
}
uint8_t CEMenuHelper::getOptionsSizeMultiplier() {
    return this->optionsSizeMultiplier;
}

void CEMenuHelper::setTitle(const char *title) {
    size_t titleSize = strlen(title) + 1;
    this->title = new char[titleSize];
    for (size_t i = 0; i < titleSize; i++) {
        this->title[i] = title[i];
    }
}
const char *CEMenuHelper::getTitle() {
    return this->title;
}

void CEMenuHelper::setHighlightBGColor(CEColor color) {
    this->highlightBGColor = color;
}
void CEMenuHelper::setHighlightTextColor(CEColor color) {
    this->highlightTextColor = color;
}
void CEMenuHelper::setTextColor(CEColor color) {
    this->textColor = color;
}
CEColor CEMenuHelper::getHighlightBGColor() {
    return this->highlightBGColor;
}
CEColor CEMenuHelper::getHighlightTextColor() {
    return this->highlightTextColor;
}
CEColor CEMenuHelper::getTextColor() {
    return this->textColor;
}

void CEMenuHelper::clearOptions() {
    for (size_t i = 0; i < this->options->size(); i++) {
        delete this->options->at(i).label;
    }
    this->options->clear();
    this->selected = 0;
}

void CEMenuHelper::setRotation(uint16_t rotation) {
   this->rotation = 0; // Sem suporte a rotação
}

void CEMenuHelper::setMirrored(bool mirrored) {
    this->mirrored = false; // Sem suporte a espelhamento
}

void CEMenuHelper::setNegative(bool negative) {
    this->negative = false; // Sem suporte a negativo
}

void CEMenuHelper::addOption(const char *label, uint id) {
    size_t labelSize = 0;
    for (labelSize = 0; *(label+labelSize) != '\0'; labelSize++);
    char *copyLabel = (char *) malloc(labelSize+1);
    for (size_t i = 0; i < labelSize; i++)
    {
        *(copyLabel + i) = *(label + i);
    }
    *(copyLabel + labelSize) = '\0';
    
    this->options->push_back({copyLabel, id});
}

void CEMenuHelper::setWidth(unsigned int w) {
    this->width = w;
}

void CEMenuHelper::setHeight(unsigned int h) {
    this->height = h;
}

unsigned int CEMenuHelper::getWidth() {
    return this->width;
}

unsigned int CEMenuHelper::getHeight() {
    return this->height;
}

uint CEMenuHelper::getSelected() {
    if(this->options->size() <= 0 || !this->selectionMade) {
        return 0;
    }
    return this->options->at(this->selected).id;
}

bool CEMenuHelper::handleControls() {
    if(this->selectionMade) {
        return true;
    }

    gpio_num_t clicked = this->ctrl->waitClick();

    switch (clicked) {
        case CLEYTIN_BTN_DOWN:
            this->moveCursorDown();
            return false;
            break;
        case CLEYTIN_BTN_UP:
            this->moveCursorUp();
            return false;
            break;
        case CLEYTIN_BTN_A:
            this->selectionMade = true;
            return true;
            break;
        case CLEYTIN_BTN_START:
            this->btnStartState = true;
            return false;
            break;

        default:
            return false;
            break;
    }
}

bool CEMenuHelper::startPressed() {
    return this->btnStartState;
}

void CEMenuHelper::moveCursorDown() {
    this->selected++;
    if(this->selected >= this->options->size()) {
        this->selected = 0;
        this->itemsOffset = 0;
    } else {
        size_t maxItems = this->getMaxItemsOnCanvas();
        while(this->selected >= (this->itemsOffset + maxItems)) {
            this->itemsOffset++;
        }
    }
}

void CEMenuHelper::moveCursorUp() {
    if(this->selected <= 0) {
        size_t maxItems = this->getMaxItemsOnCanvas();
        this->selected = this->options->size() >= 1 ? this->options->size()-1 : 0;
        this->itemsOffset = 0;
        if(this->options->size() > maxItems) {
            this->itemsOffset = this->options->size() - maxItems;
        }
    } else {
        this->selected--;
        while(this->selected < this->itemsOffset) {
            this->itemsOffset--;
        }
    }
}

CERenderWindow* CEMenuHelper::getRenderWindow() {
    CERenderWindow *window = this->getDefaultRenderWindow();
    CEPoint *start = new CEPoint((int) this->posX, (int) this->posY);
    CEPoint *end = new CEPoint((int) (this->posX + this->width), (int) (this->posY + this->height));
    window->setPoints(start, end);
    delete start;
    delete end;
    return window;
}

bool CEMenuHelper::renderToCanvas(CECanvas *canvas, CERenderWindow *window) {
    bool r = true;
    if(!this->renderOptionsToCanvas(canvas, window)) {
        r = false;
    }
    if(!this->renderMenuTitleToCanvas(canvas, window)) {
        r = false;
    }
    if(!this->renderMenuInfoToCanvas(canvas, window)) {
        r = false;
    }
    return r;
}

bool CEMenuHelper::renderOptionsToCanvas(CECanvas *canvas, CERenderWindow *window) {
    size_t maxItems = this->getMaxItemsOnCanvas();
    CEContainer *c = new CEContainer();
    c->setPositioningStyle(CEPositioningStyle::FLEX_COLUMN);
    c->setAlignY(CEAlign::START);
    c->setAlignX(CEAlign::START);
    if(this->getTitle() != NULL) {
        c->setHeight(this->getHeight() - 15); // 15 pixels do título
        c->setPosY(this->getPosY() + 15);
    } else {
        c->setHeight(this->getHeight());
        c->setPosY(this->getPosY());
    }
    c->setWidth(this->getWidth() - 8); // 8 pixels para as instruções a esquerda do menu
    c->setPosX(this->getPosX() + 8);
    for (size_t i = this->itemsOffset; i < this->options->size() && i < maxItems + this->itemsOffset; i++) {
        CEText *txt = new CEText();
        txt->setSizeMultiplier(this->getOptionsSizeMultiplier());
        txt->setBaseColor(this->getTextColor());
        txt->setText(this->options->at(i).label);
        txt->setPriority(i);
        txt->setWrap(false);
        if(i == this->selected) {
            txt->setBaseColor(this->getHighlightTextColor());
            txt->setBGColor(this->getHighlightBGColor());
        }
        c->addObject(txt);
    }
    bool r = ((CEGraphicObject *)c)->renderToCanvas(canvas);
    delete c;
    return r;
}

unsigned int CEMenuHelper::getOptionHeight() {
    CEText *txt = new CEText();
    txt->setSizeMultiplier(this->getOptionsSizeMultiplier());
    unsigned int r = txt->getHeight();
    delete txt;
    return r;
}

bool CEMenuHelper::renderMenuTitleToCanvas(CECanvas *canvas, CERenderWindow *window) {
    if(this->getTitle() == NULL) {
        return true;
    }

    CEContainer *cTitle = new CEContainer();
    cTitle->setPositioningStyle(CEPositioningStyle::FLEX_ROW);
    cTitle->setAlignY(CEAlign::CENTER);
    cTitle->setAlignX(CEAlign::CENTER);
    cTitle->setHeight(12);
    cTitle->setWidth(this->getWidth() - 8);
    cTitle->setPos(this->getPosX() + 8, this->getPosY());

    CERectangle *rect = new CERectangle();
    rect->setHeight(1);
    rect->setWidth(this->getWidth() - 8);
    rect->setPos(this->getPosX() + 8, this->getPosY() + 13);
    rect->setFilled(true);
    rect->setBaseColor(this->getBaseColor());

    CEText *txt = new CEText();
    txt->setText(this->getTitle());
    txt->setBaseColor(this->getBaseColor());
    cTitle->addObject(txt);

    bool rTitle = ((CEGraphicObject *)cTitle)->renderToCanvas(canvas);
    bool rRect = ((CEGraphicObject *)rect)->renderToCanvas(canvas);
    delete cTitle;
    delete rect;
    return rTitle && rRect;
}

size_t CEMenuHelper::getMaxItemsOnCanvas() {
    unsigned int optionsAvailableHeight = this->getHeight();
    if(this->getTitle() != NULL) {
        optionsAvailableHeight -= 15; // Abrindo espaço para o título
    }
    return optionsAvailableHeight / this->getOptionHeight();
}

bool CEMenuHelper::renderMenuInfoToCanvas(CECanvas *canvas, CERenderWindow *window) {
    size_t maxItems = this->getMaxItemsOnCanvas();
    CEContainer *c = new CEContainer();
    c->setPositioningStyle(CEPositioningStyle::FLEX_COLUMN);
    c->setAlignY(CEAlign::SPACE_BETWEEN);
    c->setAlignX(CEAlign::CENTER);
    c->setHeight(this->getHeight());
    c->setWidth(8);
    c->setPos(this->getPosX(), this->getPosY());

    if(this->selected >= maxItems) {
        CEBitmap *bmp = new CEBitmap();
        bmp->setBaseColor(this->getBaseColor());
        const uint8_t buffStack[] = {0x0, 0x10, 0x38, 0x7C, 0x7C, 0x38, 0x38, 0x38, 0x0, 0x0, 0x0, 0x0};
        uint8_t *buff = (uint8_t *) malloc(8*12);
        for (size_t i = 0; i < 12; i++) {
            *(buff+i) = buffStack[i];
        }
        bmp->setBuffer(buff);
        bmp->setHeight(12);
        bmp->setWidth(8);
        bmp->setPriority(0);
        c->addObject(bmp);
    } else {
        CEContainer *placeholder = new CEContainer();
        placeholder->setWidth(1);
        placeholder->setHeight(12);
        placeholder->setPriority(0);
        c->addObject(placeholder);
    }

    if((this->itemsOffset + maxItems) < this->options->size()) {
        CEBitmap *bmp = new CEBitmap();
        bmp->setBaseColor(this->getBaseColor());
        const uint8_t buffStack[] = {0x0, 0x0, 0x0, 0x0, 0x38, 0x38, 0x38, 0x7C, 0x7C, 0x38, 0x10, 0x0};
        uint8_t *buff = (uint8_t *) malloc(8*12);
        for (size_t i = 0; i < 12; i++) {
            *(buff+i) = buffStack[i];
        }
        bmp->setBuffer(buff);
        bmp->setHeight(12);
        bmp->setWidth(8);
        bmp->setPriority(2);
        c->addObject(bmp);
    } else {
        CEContainer *placeholder = new CEContainer();
        placeholder->setWidth(1);
        placeholder->setHeight(12);
        placeholder->setPriority(2);
        c->addObject(placeholder);
    }

    bool r = ((CEGraphicObject *)c)->renderToCanvas(canvas);
    delete c;
    return r;
}
