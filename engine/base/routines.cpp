/*
Author: Vladimir Slav

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "routines.h"

#include <stdlib.h>
#include <time.h>

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

int EngineRoutines::GetRand(int min, int max)
{
    int res = min + rand() % (max - min + 1);
    return res;
}

void EngineRoutines::InitRand()
{
    srand(static_cast<int>(time(NULL)));
}
