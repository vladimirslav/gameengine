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
    };

    class MovingParticle : public Particle
    {
    protected:
        int dx;
        int dy;
    public:
        MovingParticle(sprite_id _texture, int _x, int _y, int _life, int _dx, int _dy, int _time);
        virtual void Update(int new_time);
    };

    void Update(int time);
    void Add(Particle* p);
    void Draw(Graph* gui);

}

#endif