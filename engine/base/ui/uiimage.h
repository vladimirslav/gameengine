#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "../uiobject.h"
#include <string>

class UiImage : public UiObject
{
protected:
    sprite_id sprite;
    bool showPart;
    SDL_Rect displayedArea;
public:
    UiImage(int x,
        int y,
        size_t w,
        size_t h,
        Graph& g,
        SDL_Color borderColor,
        size_t borderWidth,
        sprite_id sprite);

    UiImage(int x,
        int y,
        size_t w,
        size_t h,
        Graph& g,
        SDL_Color borderColor,
        SDL_Rect  displayedArea,
        size_t borderWidth,
        sprite_id sprite);

    virtual void Draw();
};

#endif