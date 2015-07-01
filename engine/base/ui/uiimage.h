#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "../uiobject.h"
#include <string>

class UiImage : public UiObject
{
protected:
    sprite_id sprite;
public:
    UiImage(int x,
        int y,
        size_t w,
        size_t h,
        Graph& g,
        SDL_Color borderColor,
        size_t borderWidth,
        sprite_id sprite);

    virtual void Draw();
};

#endif