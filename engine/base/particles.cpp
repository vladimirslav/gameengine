#include "particles.h"
#include <list>

using namespace EngineParticles;
static std::list<Particle*> particles;

void Particle::Update(int new_time)
{
    if (time == 0)
    {
    }
    else
    {
        lives -= new_time - time;
        time = new_time;
        if (lives <= 0)
        {
            dead = true;
        }
    }
}

Particle::Particle(SDL_Texture* _texture, int _x, int _y, int _life, int _time)
    : x(_x), y(_y), lives(_life), time(_time), texture(_texture)
{
    dead = false;
};

Particle::~Particle()
{
}

bool Particle::IsDead()
{
    return dead;
}

int Particle::GetX()
{
    return x;
}

int Particle::GetY()
{
    return y;
}

SDL_Rect* Particle::GetFrame()
{
    return NULL;
}

SDL_Texture* Particle::GetTexture()
{
    return texture;
}

MovingParticle::MovingParticle(SDL_Texture* _texture, int _x, int _y, int _life, int _dx, int _dy, int _time)
    : Particle(_texture, _x, _y, _life, _time), dx(_dx), dy(_dy)
{
}

void MovingParticle::Update(int new_time)
{
    x += static_cast<int>(dx * (new_time - time)/1000.0);
    y += static_cast<int>(dy * (new_time - time)/1000.0);
    Particle::Update(new_time);
}

void EngineParticles::Update(int time)
{
    for (std::list<Particle*>::iterator it=particles.begin(); it!=particles.end(); ++it)
    {
        (*it)->Update(time);
    }
}

void EngineParticles::Add(Particle* p)
{
    particles.push_back(p);
}

void EngineParticles::Draw(Graph* gui)
{
    std::list<Particle*>::iterator it= particles.begin();
    while (it != particles.end())
    {
        gui->DrawTexture((*it)->GetX(), (*it)->GetY(), (*it)->GetTexture(), (*it)->GetFrame(), 0, SDL_FLIP_NONE);
        if ((*it)->IsDead())
        {
            delete(*it);
            it = particles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

