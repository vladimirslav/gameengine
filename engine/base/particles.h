#include "graph.h"

#ifndef __PARTICLES_H__
#define __PARTICLES_H__

namespace EngineParticles
{

    class Particle
    {
    protected:
        int x;
        int y;
        int lives;
        int time;
        bool dead;

        sprite_id texture;

    public:

        virtual bool IsDead();

        int GetX();
        int GetY();

        Particle(sprite_id _texture, int _x, int _y, int _life, int _time);
        virtual sprite_id GetTexture();
        virtual SDL_Rect* GetFrame();
        virtual void Update(int new_time);
        
        virtual ~Particle();

        virtual void Draw(Graph* g);
    };

    class MovingParticle : public Particle
    {
    protected:
        int dx;
        int dy;
    public:
        MovingParticle(sprite_id _texture, int _x, int _y, int _life, int _dx, int _dy, int _time);
        virtual void Update(int new_time);
        virtual void Draw(Graph* g);
    };

    class AnimatedParticle : public MovingParticle
    {
    protected:
        size_t frameW;
        size_t frameH;

        size_t drawnW;
        size_t drawnH;

        size_t frameTime;
        size_t prevFrameSwitch;

        size_t currentFrame;
        size_t frameAmount;
        
    public:
        AnimatedParticle(sprite_id _texture,
                         int _x,
                         int _y,
                         int _life,
                         int _dx,
                         int _dy,
                         int _time,
                         size_t frame_w,
                         size_t frame_h,
                         size_t drawn_w,
                         size_t drawn_h,
                         size_t frame_time,
                         size_t frame_amount);

        virtual void Update(int new_time);
        virtual void Draw(Graph* g);
    };


    class MovingTextParticle : public EngineParticles::MovingParticle
    {
    protected:
        size_t fontId;
        std::string text;
        SDL_Color color;
    public:
        MovingTextParticle(sprite_id _texture, int _x, int _y, int _life, int _dx, int _dy, int _time, size_t fontId, std::string text, SDL_Color color);
        virtual void Draw(Graph* gui);
    };

    void Update(int time);
    void Add(Particle* p);
    void Draw(Graph* gui);
    void Clear();
}

#endif