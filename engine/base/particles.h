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
        GLfloat x;
        GLfloat y;
        int lives;
        int lifetime;
        int time;
        bool dead;

        sprite_id texture;
        particleCallback callback; // called on death

    public:

        virtual bool IsDead();

        GLfloat GetX();
        GLfloat GetY();

        Particle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, int _time, particleCallback callBack = nullptr);
        virtual void setCallback(particleCallback callback);
        virtual sprite_id GetTexture();
        virtual SDL_Rect* GetFrame();
        virtual void Update(int new_time);

        virtual ~Particle();

        virtual void Draw(Graph* g);
    };

    class TraceSilhouetteParticle : public Particle
    {
    protected:
        SDL_Rect texRect;
        SDL_Rect coordinateRect;
        SDL_RendererFlip flip;
    public:
        TraceSilhouetteParticle(sprite_id _texture, SDL_Rect& _coordinateRect, SDL_Rect& _texRect, int _life, int _time, SDL_RendererFlip flip);
        virtual void Draw(Graph* g);
    };

    class MovingParticle : public Particle
    {
    protected:
        GLfloat dx;
        GLfloat dy;
    public:
        MovingParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, GLfloat _dx, GLfloat _dy, int _time);
        virtual void Update(int new_time);
        virtual void Draw(Graph* g);
    };

    class MovingParticleConfig
    {
    public:
        int life;
        size_t time;

        GLfloat dx;
        GLfloat dy;

        GraphColor color;
        bool fadeOut;
        GLfloat minAlpha;
        GLfloat maxAlpha;

        bool scaling;
        GLfloat maxScale;
        GLfloat minScale;

        bool shake;
        GLfloat shakeX;
        GLfloat shakeY;

        GLfloat intensity;

        MovingParticleConfig(int _life)
        {
            life = _life;

            color = { 1.0f, 1.0f, 1.0f, 1.0f };
            maxScale = 1.0f;
            minScale = 1.0f;

            dx = 0;
            dy = 0;
            fadeOut = true;

            minAlpha = 1.0f;
            maxAlpha = 1.0f;

            shake = false;
            scaling = false;

            shakeX = 0;
            shakeY = 0;
            intensity = 1;
        }
    };

    class ConfigurableMovingParticle : public MovingParticle
    {
    public:
        GraphColor color;
        bool fadeOut;
        GLfloat minAlpha;
        GLfloat maxAlpha;

        bool scaling;
        GLfloat currentScale;
        GLfloat maxScale;
        GLfloat minScale;

        bool shake;
        GLfloat shakeX;
        GLfloat shakeY;

        GLfloat intensity;

        ConfigurableMovingParticle(GLfloat x, GLfloat y, size_t time, sprite_id texture, MovingParticleConfig& c);
        virtual void Draw(Graph* g);
    };

    class SparkParticle : public MovingParticle
    {
    protected:
        GraphColor color;
        bool applyPhysics;
        GLfloat ddy;
        bool useTexture;
    public:
        SparkParticle(GLfloat _x, GLfloat _y, int _life, GLfloat _dx, GLfloat _dy, int _time, const GraphColor& _color, bool applyPhysics, GLfloat ddy);
        SparkParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, GLfloat _dx, GLfloat _dy, int _time, const GraphColor& _color, bool applyPhysics, GLfloat ddy);
        virtual void Update(int new_time);
        virtual void Draw(Graph* g);
    };
    
    struct MovementNode
    {
        GLfloat targetX;
        GLfloat targetY;
        size_t moveTime;
        bool fadeOut;
    };

    using MovementNodeCollection = std::vector<MovementNode>;

    class TargetedMovingParticle : public Particle
    {
    protected:
        bool useTexture;

        size_t currentNode;
        size_t currentStartTime;
        GraphColor color;
        MovementNodeCollection nodes;
        GLfloat startx;
        GLfloat starty;
        size_t w;
        size_t h;

    public:
        TargetedMovingParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _time, MovementNodeCollection& nodes, GraphColor color);
        TargetedMovingParticle(GLfloat _x, GLfloat _y, int _time, MovementNodeCollection& nodes, GraphColor color, size_t w, size_t h);
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

        GraphColor palette;

    public:
        AnimatedParticle(sprite_id _texture,
                         GLfloat _x,
                         GLfloat _y,
                         int _life,
                         GLfloat _dx,
                         GLfloat _dy,
                         int _time,
                         size_t frame_w,
                         size_t frame_h,
                         size_t drawn_w,
                         size_t drawn_h,
                         size_t frame_time,
                         size_t frame_amount,
                         GraphColor palette);

        virtual void Update(int new_time);
        virtual void Draw(Graph* g);
    };

    class MovingTextParticle : public EngineParticles::MovingParticle
    {
    protected:
        const FontDescriptor* font;
        std::string text;
        SDL_Color color;
        SDL_Color borderColor;
        bool hasBorder;
    public:
        MovingTextParticle(GLfloat _x, GLfloat _y, int _life, GLfloat _dx, GLfloat _dy, int _time, const FontDescriptor* fontId, std::string text, SDL_Color color);
        MovingTextParticle(GLfloat _x, GLfloat _y, int _life, GLfloat _dx, GLfloat _dy, int _time, const FontDescriptor* fontId, std::string text, SDL_Color color, SDL_Color borderColor);
        virtual void Draw(Graph* gui);
    };

    class TextParticleConfig
    {
    public:
        GLfloat x;
        GLfloat y;
        int life;
        size_t time;
        const FontDescriptor* fontId;
        SDL_Color color;
        SDL_Color borderColor;
        GLfloat maxScale;
        GLfloat minScale;
        GLfloat dx;
        GLfloat dy;
        bool fadeOut;
        bool shake;
        bool scaling;
        GLfloat shakeX;
        GLfloat shakeY;

        GLfloat intensity;

        TextParticleConfig(GLfloat _x, GLfloat _y, int _life, const FontDescriptor* _fontId, size_t _time)
        {
            x = _x;
            y = _y;
            life = _life;
            time = _time;
            fontId = _fontId;

            color = { 0, 0, 0, 255 };
            borderColor = color;
            maxScale = 1.0f;
            minScale = 1.0f;
            dx = 0;
            dy = 0;
            fadeOut = true;
            shake = false;
            scaling = false;

            shakeX = 0;
            shakeY = 0;
            intensity = 1;
        }
    };

    class ConfigurableTextParticle : public EngineParticles::MovingTextParticle
    {
    protected:
        GLfloat maxScale;
        GLfloat minScale;
        bool fadeOut;
        bool shake;
        bool scaling;
        GLfloat intensity;

        GLfloat currentScale;

        GLfloat shakeDeltaX;
        GLfloat shakeDeltaY;
    public:
        ConfigurableTextParticle(TextParticleConfig& c, std::string text);
        virtual void Draw(Graph* gui);
    };

    class FadingTextParticle : public EngineParticles::Particle
    {
    protected:
        EngineTimer::CountdownTimer t;
        const FontDescriptor* font;
        std::string text;
        SDL_Color color;
        SDL_Color borderColor;
        bool hasBorder;

        size_t width;
    public:
        FadingTextParticle(GLfloat _x, GLfloat _y, int _life, int _time, const FontDescriptor* fontId, const std::string& text, SDL_Color color, size_t _width = 0);
        FadingTextParticle(GLfloat _x, GLfloat _y, int _life, int _time, const FontDescriptor* fontId, const std::string& text, SDL_Color color, SDL_Color borderColor, size_t _width = 0);
        virtual void Draw(Graph* gui);
    };

    class FadingOutPointerParticle : public EngineParticles::Particle
    {
    protected:
        EngineTimer::CountdownTimer t;

        GLfloat moveX;
        GLfloat moveY;

        GLfloat dx;
        GLfloat dy;

        GLfloat maxDx;
        GLfloat maxDy;
    public:
        FadingOutPointerParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, int _time, GLfloat maxDx, GLfloat maxDy);
        virtual void Draw(Graph* gui);
        virtual void Update(int new_time);
    };

    class LeafParticle : public EngineParticles::Particle
    {
    protected:
        GLfloat speedX;
        GLfloat speedY;
        int lifeCycle;
    public:
        LeafParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, int _time, int lifeCycle, GLfloat speedX, GLfloat speedY);
        virtual void Draw(Graph* gui);
        virtual void Update(int new_time);
    };

    void Update(int time);
    void Add(Particle* p, particleCallback cb = nullptr);
    void Draw(Graph* gui);
    void Clear();
}

#endif
