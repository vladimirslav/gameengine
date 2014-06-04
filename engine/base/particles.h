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

        SDL_Texture* texture;

    public:

        virtual bool IsDead();

        int GetX();
        int GetY();

        Particle(SDL_Texture* _texture, int _x, int _y, int _life, int _time);
        virtual SDL_Texture* GetTexture();
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
        MovingParticle(SDL_Texture* _texture, int _x, int _y, int _life, int _dx, int _dy, int _time);
        virtual void Update(int new_time);
    };

    void Update(int time);
    void Add(Particle* p);
    void Draw(Graph* gui);

}

#endif