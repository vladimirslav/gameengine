#include "input.h"

bool InputHandler::IsPressed(SDL_Scancode key)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    return state[key] != 0;
}