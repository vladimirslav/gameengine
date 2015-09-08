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

#include "graph.h"
#include "countdown.h"

#ifndef __PARTICLES_H__
#define __PARTICLES_H__

using particleCallback = void (*)(void*);

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
        particleCallback callback; // called on death

    public:

        virtual bool IsDead();

        int GetX();
        int GetY();

        Particle(sprite_id _texture, int _x, int _y, int _life, int _time, particleCallback callBack = nullptr);
        virtual void setCallback(particleCallback callback);
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
        const FontDescriptor* font;
        std::string text;
        SDL_Color color;
    public:
        MovingTextParticle(sprite_id _texture, int _x, int _y, int _life, int _dx, int _dy, int _time, const FontDescriptor* fontId, std::string text, SDL_Color color);
        virtual void Draw(Graph* gui);
    };

    class FadingTextParticle : public EngineParticles::Particle
    {
    protected:
        EngineTimer::CountdownTimer t;
        const FontDescriptor* font;
        std::string text;
        SDL_Color color;
    public:
        FadingTextParticle(int _x, int _y, int _life, int _time, const FontDescriptor* fontId, std::string& text, SDL_Color color);
        virtual void Draw(Graph* gui);
    };

    class FadingOutPointerParticle : public EngineParticles::Particle
    {
    protected:
        EngineTimer::CountdownTimer t;

        int moveX;
        int moveY;

        int dx;
        int dy;

        int maxDx;
        int maxDy;
    public:
        FadingOutPointerParticle(sprite_id _texture, int _x, int _y, int _life, int _time, int maxDx, int maxDy);
        virtual void Draw(Graph* gui);
        virtual void Update(int new_time);
    };

    void Update(int time);
    void Add(Particle* p, particleCallback cb = nullptr);
    void Draw(Graph* gui);
    void Clear();
}

#endif
