#ifndef __COMMIEENGINE_SDL_ROUTINES_H__
#define __COMMIEENGINE_SDL_ROUTINES_H__

#include <SDL.h>

namespace EngineRoutines
{

    SDL_assert_state handler(const SDL_assert_data* data,
                             void*                  userdata);

    void InitRand();
    int GetRand(int min, int max);
}
#endif