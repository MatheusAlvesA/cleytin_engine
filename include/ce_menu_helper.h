#ifndef CE_MENU_HELPER_H
#define CE_MENU_HELPER_H

#include "cleytin_engine.h"
#include "ce_container.h"
#include "ce_text.h"
#include "ce_rectangle.h"
#include "ce_bitmap.h"
#include "cleytin_controls.h"

#define FONT_WIDTH 8
#define FONT_HEIGHT 12

typedef struct {
  const char *label;
  uint id;
} CEMenuOption;

class CEMenuHelper : public CEGraphicObject {
public:
    CEMenuHelper();
    ~CEMenuHelper();
    void setWidth(unsigned int w);
    void setHeight(unsigned int h);
    unsigned int getWidth();
    unsigned int getHeight();
    uint getSelected();

    void setMirrored(bool mirrored);
    void setNegative(bool negative);
  
    void setHighlightBGColor(CEColor color);
    void setHighlightTextColor(CEColor color);
    void setTextColor(CEColor color);
    CEColor getHighlightBGColor();
    CEColor getHighlightTextColor();
    CEColor getTextColor();

    void setOptionsSizeMultiplier(uint8_t size);
    uint8_t getOptionsSizeMultiplier();

    void setTitle(const char *title);
    const char *getTitle();

    void addOption(const char *label, uint id);
    void clearOptions();

    void moveCursorDown();
    void moveCursorUp();
    bool handleControls();
    bool startPressed();
    void reset();

    CERenderWindow* getRenderWindow();
    void renderToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow);

protected:
    CleytinControls *ctrl;
    unsigned int width;
    unsigned int height;
    std::vector <CEMenuOption> *options;
    size_t selected;
    size_t itemsOffset;
    bool selectionMade;
    bool btnStartState;
    CEColor highlightBGColor;
    CEColor highlightTextColor;
    CEColor textColor;
    uint8_t optionsSizeMultiplier;
    char *title;

    void renderOptionsToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow);
    void renderMenuInfoToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow);
    void renderMenuTitleToCanvas(CECanvas *canvas, CERenderWindow *window, CERenderWindow *subWindow);
    unsigned int getOptionHeight();
    size_t getMaxItemsOnCanvas(); 
};

#endif
