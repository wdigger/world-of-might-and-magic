#pragma once

#include "GUI/GUIWindow.h"

class GUIWindow_Rest : public GUIWindow {
 public:
    GUIWindow_Rest();
    virtual ~GUIWindow_Rest() {}

    virtual void Update();
};

class GUIWindow_RestWindow : public GUIWindow {
 public:
    GUIWindow_RestWindow(unsigned int x, unsigned int y, unsigned int width, unsigned int height, void *pButton, const char *hint) :
        GUIWindow(x, y, width, height, pButton, hint)
    {}
    virtual ~GUIWindow_RestWindow() {}

    virtual void Update();
    virtual void Release();
};

extern class Image *rest_ui_sky_frame_current;
extern class Image *rest_ui_hourglass_frame_current;
