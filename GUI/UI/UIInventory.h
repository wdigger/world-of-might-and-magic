#pragma once

#include "GUI/GUIWindow.h"

class GUIWindow_Inventory : public GUIWindow {
 public:
    GUIWindow_Inventory(unsigned int x, unsigned int y, unsigned int width, unsigned int height, GUIButton *pButton, const String &hint = String()) :
        GUIWindow(x, y, width, height, pButton, hint) {}
    virtual ~GUIWindow_Inventory() {}

    virtual void Update();
};

class GUIWindow_Inventory_CastSpell : public GUIWindow {
 public:
    GUIWindow_Inventory_CastSpell(unsigned int x, unsigned int y, unsigned int width, unsigned int height, void *pButton, const String &hint = String());
    virtual ~GUIWindow_Inventory_CastSpell() {}

    virtual void Update();
};
