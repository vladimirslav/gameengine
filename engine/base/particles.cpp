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

Particle::Particle(sprite_id _texture, int _x, int _y, int _life, int _time, particleCallback callBack)
    : x(_x), y(_y), lives(_life), time(_time), texture(_texture), callback(callBack)
{
    dead = false;
};

Particle::~Particle()
{
    if (callback != nullptr)
    {
        callback(nullptr);
    }
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

void Particle::setCallback(particleCallback callback)
{
    this->callback = callback;
}

void Particle::Draw(Graph* g)
{

}

sprite_id Particle::GetTexture()
{
    return texture;
}

MovingParticle::MovingParticle(sprite_id _texture, int _x, int _y, int _life, int _dx, int _dy, int _time)
    : Particle(_texture, _x, _y, _life, _time), dx(_dx), dy(_dy)
{
}

void MovingParticle::Update(int new_time)
{
    x += static_cast<int>(dx * (new_time - time)/1000.0);
    y += static_cast<int>(dy * (new_time - time)/1000.0);
    Particle::Update(new_time);
}

void MovingParticle::Draw(Graph* gui)
{
    gui->DrawTexture(GetX(), GetY(), gui->GetTexture(GetTexture()), GetFrame(), 0, SDL_FLIP_NONE);
}

MovingTextParticle::MovingTextParticle(sprite_id _texture, int _x, int _y, int _life, int _dx, int _dy, int _time, size_t fontId, std::string text, SDL_Color color)
: MovingParticle(_texture, _x, _y, _life, _dx, _dy, _time)
, fontId(fontId)
, text(text)
, color(color)
{
}

void MovingTextParticle::Draw(Graph* gui)
{
    gui->WriteNormal(fontId, text, x, y, color);
}

AnimatedParticle::AnimatedParticle(sprite_id _texture,
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
                                   size_t frame_amount)
    : MovingParticle(_texture, _x, _y, _life, _dx, _dy, _time)
    , frameW(frame_w)
    , frameH(frame_h)
    , drawnW(drawn_w)
    , drawnH(drawn_h)
    , frameTime(frame_time)
    , prevFrameSwitch(_time)
    , currentFrame(0)
    , frameAmount(frame_amount)
{
}

void AnimatedParticle::Update(int new_time)
{
    if (new_time - prevFrameSwitch > frameTime)
    {
        prevFrameSwitch = new_time;
        currentFrame++;
        if (currentFrame >= frameAmount)
        {
            currentFrame = 0;
        }
    }
    MovingParticle::Update(new_time);
}

void AnimatedParticle::Draw(Graph* g)
{
    SDL_Rect frame{GetX(), GetY(), static_cast<int>(drawnW), static_cast<int>(drawnH)};
    SDL_Rect framePart{ static_cast<int>(currentFrame * frameW), 0, static_cast<int>(frameW), static_cast<int>(frameH) };
    g->DrawTexture(&frame, texture, &framePart, 0, SDL_FLIP_NONE);
}

void EngineParticles::Update(int time)
{
    for (std::list<Particle*>::iterator it=particles.begin(); it!=particles.end(); ++it)
    {
        (*it)->Update(time);
    }
}

void EngineParticles::Add(Particle* p, particleCallback cb)
{
    particles.push_back(p);
    if (cb != nullptr)
    {
        p->setCallback(cb);
    }
}

void EngineParticles::Draw(Graph* gui)
{
    std::list<Particle*>::iterator it= particles.begin();
    while (it != particles.end())
    {
        (*it)->Draw(gui);
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

void EngineParticles::Clear()
{
    for (auto it : particles)
    {
        delete it;
    }

    particles.clear();
}

FadingOutPointerParticle::FadingOutPointerParticle(sprite_id _texture, int _x, int _y, int _life, int _time, int maxDx, int maxDy)
    : Particle(_texture, _x, _y, _life, _time)
    , t(_life)
    , moveX(0)
    , moveY(0)
    , dx(0)
    , dy(0)
    , maxDx(maxDx)
    , maxDy(maxDy)
{
    if (maxDx != 0)
    {
        dx = (maxDx > 0) ? 1 : -1;
    }

    if (maxDy != 0)
    {
        dy = (maxDy > 0) ? 1 : -1;
    }
}

void FadingOutPointerParticle::Draw(Graph* gui)
{
    gui->PushAlpha(static_cast<Uint8>(255 * t.RemainingPart()));
    gui->DrawTexture(x + moveX, y + moveY, texture);
    gui->PopAlpha();
}

void FadingOutPointerParticle::Update(int new_time)
{
    if (maxDx > 0)
    {
        if (x + moveX >= x + maxDx)
        {
            dx = -1;
        }
        else if (x + moveX <= x)
        {
            dx = 1;
        }
    }
    else if (maxDx < 0)
    {
        if (x + moveX <= x + maxDx)
        {
            dx = 1;
        }
        else if (x + moveX >= x)
        {
            dx = -1;
        }
    }

    if (maxDy > 0)
    {
        if (y + moveY >= y + maxDy)
        {
            dy = -1;
        }
        else if (y + moveY <= y)
        {
            dy = 1;
        }
    }
    else if (maxDy < 0)
    {
        if (y + moveY <= y + maxDy)
        {
            dy = 1;
        }
        else if (y + moveY >= y)
        {
            dy = -1;
        }
    }

    moveX += dx;
    moveY += dy;
    Particle::Update(new_time);
}
