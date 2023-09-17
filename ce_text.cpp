#include "ce_text.h"

CEText::CEText() {
    this->font = new CE8x12Font();
    this->text = new char[1];
    this->text[0] = '\0';
    this->length = 0;
    this->wrap = true;
    this->sizeMultiplier = 1;
    this->bgColor = {0xFF, 0xFF, 0xFF};
    this->bgColorSet = false;
    this->lengthWidestLine = 0;
    this->lineBreaksCount = 0;
    this->addCurrentWindowAsAltered();
}

CEText::~CEText() {
    delete this->font;
    delete this->text;
}

void CEText::recalcLineBreaks() {
    size_t availableWidth = this->maxX - this->posX;
    size_t charWidth = this->font->getCharWidth() * this->getSizeMultiplier();
    size_t currentLineLength = 0;
    this->lengthWidestLine = 0;
    this->lineBreaksCount = 0;
    for (size_t i = 0; this->text[i] != '\0'; i++) {
        currentLineLength++;
        if(currentLineLength > this->lengthWidestLine) {
            this->lengthWidestLine = currentLineLength;
        }
        if(
            this->text[i] == '\n' ||
            (((currentLineLength + 1) * charWidth > availableWidth) && this->wrap)
        ) {
            this->lineBreaksCount++;
            currentLineLength = 0;
            continue;
        }
    }

    if(currentLineLength > this->lengthWidestLine) {
        this->lengthWidestLine = currentLineLength;
    }
    if((currentLineLength * charWidth > availableWidth) && this->wrap) {
        this->lineBreaksCount++;
    }
}

void CEText::setCustomFont(CEFont *font) {
    delete this->font;
    this->font = font;
    this->addCurrentWindowAsAltered();
}

void CEText::setText(const char *buffer) {
    delete this->text;
    this->text = this->font->parseString(buffer);
    char *tmp = this->text;
    for (this->length = 0; *(tmp++) != '\0'; this->length++);
    this->addCurrentWindowAsAltered();
}

void CEText::setWrap(bool wrap) {
    this->wrap = wrap;
    this->addCurrentWindowAsAltered();
}

void CEText::setBGColor(CEColor color) {
    this->bgColor = color;
    this->bgColorSet = true;
    this->addCurrentWindowAsAltered();
}

void CEText::addCurrentWindowAsAltered()
{
    this->recalcLineBreaks();
    CEGraphicObject::addCurrentWindowAsAltered();
}

CEColor CEText::getBGColor() {
    return this->bgColor;
}

unsigned int CEText::getWidth() {
    size_t charWidth = this->font->getCharWidth() * this->getSizeMultiplier();
    unsigned int width = this->lengthWidestLine * charWidth;
    if(this->posX + width >= this->maxX) {
        return this->maxX - this->posX;
    } else {
        return width;
    }
}

unsigned int CEText::getHeight() {
    size_t charHeight = this->font->getCharHeight() * this->getSizeMultiplier();
    return charHeight * (this->lineBreaksCount + 1);
}

uint8_t CEText::getSizeMultiplier() {
    return this->sizeMultiplier;
}

void CEText::setSizeMultiplier(uint8_t size) {
    this->sizeMultiplier = size;
    this->addCurrentWindowAsAltered();
}

CERenderWindow* CEText::getRenderWindow() {
    CERenderWindow *window = this->getDefaultRenderWindow();
    CEPoint *start = new CEPoint(this->posX, this->posY);
    CEPoint *end = new CEPoint((this->posX + (int) this->getWidth()), (this->posY + (int) this->getHeight()));
    window->setPoints(start, end);
    delete start;
    delete end;
    return window;
}

bool CEText::renderChar(CECanvas *canvas, char c, int x, int y) {
    uint8_t *mappedPointer = this->font->getRawPonter() + this->font->getPositionOf(c);
    unsigned int charWidth = this->font->getCharWidth();
    unsigned int charHeight = this->font->getCharHeight();
    bool r = true;
    CEColor currentBGColor = this->bgColorSet ? this->getBGColor() : canvas->getBackgroundColor();
    for (size_t cursorY = 0; cursorY < charHeight; cursorY++) {
        for (size_t cursorX = 0; cursorX < charWidth; cursorX++) {
            unsigned int bitPos = cursorX + (cursorY * charWidth);
            unsigned int bytePos = bitPos / 8;
            unsigned int bitOffset = bitPos % 8;
            for (size_t i = 0; i < this->getSizeMultiplier(); i++) {
                for (size_t j = 0; j < this->getSizeMultiplier(); j++) {
                    uint8_t activePixel = mappedPointer[bytePos] & (1 << (7 - bitOffset));
                    if(activePixel == 0 && !this->bgColorSet) {
                        continue;
                    }
                    if(!this->setPixel(
                        canvas,
                        i + x + (cursorX * this->getSizeMultiplier()),
                        j + y + (cursorY * this->getSizeMultiplier()),
                        activePixel ? this->getBaseColor() : currentBGColor
                    )) {
                        r = false;
                    }
                }
            }
        }
    }
    return r;
}

bool CEText::renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow) {
    CERenderWindow *w = this->getRenderWindow();
    int startX = w->topLeft->x;
    int cursorX = startX;
    int cursorY = w->topLeft->y;
    unsigned int maxX = w->bottomRight->x;
    unsigned int maxY = w->bottomRight->y;
    delete w;

    size_t charWidth = (size_t) this->font->getCharWidth();
    charWidth *= this->getSizeMultiplier();
    size_t charHeight = (size_t) this->font->getCharHeight();
    charHeight *= this->getSizeMultiplier();
    bool allRendered = true;
    for (size_t i = 0; this->text[i] != '\0'; i++) {
        if(this->text[i] == '\n') { // Situação especial, quebra de linha
            size_t nextCharYPos = 0;
            if(!this->calcBreakLine(cursorY, cursorX, startX, charHeight, maxY, nextCharYPos)) {
                return false;
            }
            continue;
        }
        if(!this->renderChar(canvas, this->text[i], cursorX, cursorY)) {
            allRendered = false;
        }
        if(this->text[i+1] == '\n') { // O próximo char é uma quebra de linha
            continue; // Não calcule a posição do próximo char, pois ele não renderizará
        }
        size_t nextCharXPos = ((size_t) cursorX) + charWidth; // Determine a posição do próximo char na linha

        if(nextCharXPos + charWidth <= maxX) { // Ainda cabe todo o próximo caractere nessa linha
            cursorX = nextCharXPos; // Avance na linha
            continue; // Prossiga para renderizar o próximo char
        }
        // Não cabe todo o próximo char nessa linha
        if(!this->wrap) { // Se precisa renderizar tudo em uma linha
            if(nextCharXPos <= maxX) { // Se cabe ao menos uma parte
                cursorX = nextCharXPos; // Avance na linha
                continue; // Prossiga para renderizar o próximo char
            }
            return false; // Não é mais possível renderizar nada na linha, terminando
        }
        size_t nextCharYPos = 0;
        if(!this->calcBreakLine(cursorY, cursorX, startX, charHeight, maxY, nextCharYPos)) {
            return false;
        }
    }

    return allRendered;
}

bool CEText::calcBreakLine(
    int &cursorY,
    int &cursorX,
    int startX,
    size_t charHeight,
    unsigned int maxY,
    size_t &nextCharYPos
) {
    // Realizando retorno de carrosel (Wrap)
    nextCharYPos = ((size_t) cursorY) + charHeight; // Determine a posição y da próxima linha
    if(nextCharYPos > maxY) { // Se o início da próxima linha está fora do limite
        return false; // Termine o render
    }
    cursorY = nextCharYPos; // Vá para a próxima linha
    cursorX = startX; // Comece do início da próxima linha

    return true;
}
