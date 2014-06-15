#ifndef __INPUT_H__
#define __INPUT_H__

#include <SDL.h>

class InputHandler
{
public:
    static bool IsPressed(SDL_Scancode key);
};

#endif