#include <stdio.h>
#include "..\base\routines.h"
#include "..\base\graph.h"
#include "..\base\Timer.h"
#include "..\base\particles.h"

static const int FRAMES_PER_SECOND = 60;
static const int SCREEN_H = 480;
static const int SCREEN_W = 640;

static const int SPEED = 5;

class DustParticle : public EngineParticles::Particle
{
private:
    unsigned int frame;
    unsigned int frame_time; //How fast do frames flip
    unsigned int last_time;

    unsigned short frames_horizontal;
    unsigned short frames_vertical;

    unsigned short frame_w;
    unsigned short frame_h;

    unsigned short last_frame;

    SDL_Rect frame_r;
public:

    DustParticle(SDL_Texture* _texture, int _x, int _y, int _life, int _time)
        : Particle(_texture, _x, _y, _life, _time)
        , frame(0)
        , frame_time(100)
        , last_time(_time)
        , frames_horizontal(6)
        , frames_vertical(1)
    {
        int w = 0;
        int h = 0;

        SDL_assert_release(SDL_QueryTexture(texture, NULL, NULL, &w, &h) == 0);

        frame_w = w / frames_horizontal;
        frame_h = h / frames_vertical;

        frame_r.w = frame_w;
        frame_r.h = frame_h;

        last_frame = frames_horizontal * frames_vertical - 1;
    }

    virtual void Update(int new_time)
    {
        Particle::Update(new_time);
        if (new_time - last_time > frame_time)
        {
            last_time = new_time;
            frame++;
        }
    }

    virtual bool IsDead()
    {
        return frame > last_frame;
    }

    SDL_Rect* GetFrame()
    {

        if (frame < last_frame)
        {
            frame_r.x = (frame % frames_horizontal) * frame_w;
            frame_r.y = (frame / frames_horizontal) * frame_h;
        }
        else
        {
            frame_r.x = (last_frame % frames_horizontal) * frame_w;
            frame_r.y = (last_frame / frames_horizontal) * frame_h;
        }

        return &frame_r;
    }


};

int main(int argc, char** argv){
    SDL_SetAssertionHandler(EngineRoutines::handler, NULL);
    Graph g(SCREEN_W, SCREEN_H, "", "test");
    Timer timer;
    timer.Reset();


    SDL_Texture* basilisk = g.LoadTextureAlphaPink("res\\sprite\\basilisk.png");
    SDL_Texture* dust = g.LoadTextureAlphaPink("res\\sprite\\dust.png");
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

    Timer particle_timer;
    particle_timer.Reset();

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
                case SDLK_q:
                    EngineParticles::Add(new DustParticle(dust, 100, 100, 1, particle_timer.GetTicks()));
                }
                break;
            }
        }

        EngineParticles::Update(particle_timer.GetTicks());

        g.ClrScr();
        EngineParticles::Draw(&g);
        g.DrawTexture(p_x, p_y, basilisk, NULL, face_right ? -angle : angle, face_right ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        g.Flip();
    }
    SDL_DestroyTexture(basilisk);
    SDL_DestroyTexture(dust);
    return 0;
}