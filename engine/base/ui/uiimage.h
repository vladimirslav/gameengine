#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "../uiobject.h"
#include "../sprite.h"
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

class UiAnimatedImage : public UiImage
{
protected:
    animation_list anims;
    Sprite img;
    Timer spriteTimer;
public:
    UiAnimatedImage(int x, int y, size_t w, size_t h, Graph& g, size_t frames, size_t frameTime, const char* fName);
    virtual void Draw();
};

#endif