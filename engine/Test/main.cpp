#include <stdio.h>
#include "..\base\routines.h"
#include "..\base\graph.h"
#include "..\base\Timer.h"

static const int FRAMES_PER_SECOND = 60;
static const int SCREEN_H = 480;
static const int SCREEN_W = 640;

static const int SPEED = 5;

int main(int argc, char** argv){
    SDL_SetAssertionHandler(EngineRoutines::handler, NULL);
    Graph g(SCREEN_W, SCREEN_H, "", "test");
    Timer timer;
    timer.Reset();

    SDL_Texture* basilisk = g.LoadTextureAlphaPink("res\\sprite\\basilisk.png");
    SDL_Color bgcolor;
    bgcolor.a = 255;
    bgcolor.r = 0;
    bgcolor.g = 127;
    bgcolor.b = 0;
    g.SetBgColor(bgcolor);
    bool quit = false;

    int p_x = 0;
    int p_y = 0;

    double face_right = false;
    double angle = 45;

    while (!quit)
    {
        timer.Reset();
        if (timer.GetTicks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - timer.GetTicks());
        }
        SDL_PumpEvents();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_w:
                    angle = 45;
                    if (p_y - SPEED >= 0)
                    {
                        p_y -= SPEED;
                    }
                    break;
                case SDLK_s:
                    angle = -45;
                    if (p_y + 54 + SPEED <= SCREEN_H)
                    {
                        p_y += SPEED;
                    }
                    break;
                case SDLK_d:
                    face_right = true;
                    if (p_x + 54 + SPEED <= SCREEN_W)
                    {
                        p_x += SPEED;
                    }
                    break;
                case SDLK_a:
                    face_right = false;
                    if (p_x - SPEED >= 0)
                    {
                        p_x -= SPEED;
                    }
                    break;
                }
                break;
            }
        }

        g.ClrScr();
        g.DrawTexture(p_x, p_y, basilisk, NULL, face_right ? -angle : angle, face_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        g.Flip();
    }
    SDL_DestroyTexture(basilisk);
    return 0;
}