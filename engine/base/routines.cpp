#include "routines.h"

SDL_assert_state EngineRoutines::handler(const SDL_assert_data* data,
                                         void*                  userdata)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "An error has occured",
        SDL_GetError(),
        NULL);
#ifdef _DEBUG
    return SDL_ASSERTION_BREAK;
#else
    return SDL_ASSERTION_IGNORE;
#endif
}
