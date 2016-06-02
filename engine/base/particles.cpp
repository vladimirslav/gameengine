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
            lives = 0;
            dead = true;
        }
    }
}

Particle::Particle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, int _time, particleCallback callBack)
    : x(_x), y(_y), lives(_life), time(_time), lifetime(_life), texture(_texture), callback(callBack)
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

GLfloat Particle::GetX()
{
    return x;
}

GLfloat Particle::GetY()
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

MovingParticle::MovingParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, GLfloat _dx, GLfloat _dy, int _time)
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
    SDL_Rect r{ (int)GetX(), (int)GetY(), 0, 0 };
    gui->DrawTexture(&r, GetTexture(), GetFrame(), 0, SDL_FLIP_NONE);
}

SparkParticle::SparkParticle(GLfloat _x, GLfloat _y, int _life, GLfloat _dx, GLfloat _dy, int _time, const GraphColor& _color, bool _applyPhysics, GLfloat _ddy)
    : MovingParticle(0, _x, _y, _life, _dx, _dy, _time)
    , color(_color)
    , applyPhysics(_applyPhysics)
    , ddy(_ddy)
    , useTexture(false)
{
}

SparkParticle::SparkParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, GLfloat _dx, GLfloat _dy, int _time, const GraphColor& _color, bool _applyPhysics, GLfloat _ddy)
    : MovingParticle(_texture, _x, _y, _life, _dx, _dy, _time)
    , color(_color)
    , applyPhysics(_applyPhysics)
    , ddy(_ddy)
    , useTexture(true)
{
}


void SparkParticle::Draw(Graph* g)
{
    if (texture != 0)
    {
        g->PushAlpha(color.a);
        g->PushTextureColorValues(color);
        g->DrawTexture(x, y, texture);
        g->PopTextureColorValue();
        g->PopAlpha();
    }
    else
    {
        g->DrawRect((int)GetX(), (int)GetY(), 2, 2, color);
    }
}

void SparkParticle::Update(int new_time)
{
    color.a = lives / (GLfloat)lifetime;
    if (applyPhysics)
    {
        // dy -gt
        dy += ddy * ((new_time - time) / (float)1000);
    }
    MovingParticle::Update(new_time);
}

MovingTextParticle::MovingTextParticle(GLfloat _x,
                                       GLfloat _y,
                                       int _life,
                                       GLfloat _dx,
                                       GLfloat _dy,
                                       int _time,
                                       const FontDescriptor* fontId, 
                                       std::string text, 
                                       SDL_Color color)
: MovingParticle(0, _x, _y, _life, _dx, _dy, _time)
, font(fontId)
, text(text)
, color(color)
, hasBorder(false)
{
}

MovingTextParticle::MovingTextParticle(GLfloat _x,
                                       GLfloat _y,
                                       int _life,
                                       GLfloat _dx,
                                       GLfloat _dy,
                                       int _time,
                                       const FontDescriptor* fontId,
                                       std::string text,
                                       SDL_Color color,
                                       SDL_Color borderColor)
    : MovingParticle(0, _x, _y, _life, _dx, _dy, _time)
    , font(fontId)
    , text(text)
    , color(color)
    , borderColor(borderColor)
    , hasBorder(true)
{
}

void MovingTextParticle::Draw(Graph* gui)
{
    color.a = (Uint8)(255 * (1.0f - (GLfloat)lives / lifetime));
    //gui->PushAlpha((1.0f - (GLfloat)lives / lifetime));
    if (hasBorder)
    {
        gui->WriteBorderedText(*font, text, x, y, color, borderColor);
    }
    else
    {
        gui->WriteNormal(*font, text, (int)x, (int)y, color);
    }
    //gui->PopAlpha();
}

AnimatedParticle::AnimatedParticle(sprite_id _texture,
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
                                   GraphColor _palette)
    : MovingParticle(_texture, _x, _y, _life, _dx, _dy, _time)
    , frameW(frame_w)
    , frameH(frame_h)
    , drawnW(drawn_w)
    , drawnH(drawn_h)
    , frameTime(frame_time)
    , prevFrameSwitch(_time)
    , currentFrame(0)
    , frameAmount(frame_amount)
    , palette(_palette)
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
    g->PushAlpha(palette.a);
    g->PushTextureColorValues(palette);
    SDL_Rect frame{(int)GetX(), (int)GetY(), static_cast<int>(drawnW), static_cast<int>(drawnH)};
    SDL_Rect framePart{ static_cast<int>(currentFrame * frameW), 0, static_cast<int>(frameW), static_cast<int>(frameH) };
    g->DrawTexture(&frame, texture, &framePart, 0, SDL_FLIP_NONE);
    g->PopTextureColorValue();
    g->PopAlpha();
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

FadingTextParticle::FadingTextParticle(GLfloat _x, GLfloat _y, int _life, int _time, const FontDescriptor* fontId, const std::string& text, SDL_Color color, size_t _width)
    : Particle(0, _x, _y, _life, _time)
    , font(fontId)
    , text(text)
    , color(color)
    , t(_life)
    , hasBorder(false)
    , width(_width)
{

}

FadingTextParticle::FadingTextParticle(GLfloat _x, GLfloat _y, int _life, int _time, const FontDescriptor* fontId, const std::string& text, SDL_Color color, SDL_Color _borderColor, size_t _width)
    : Particle(0, _x, _y, _life, _time)
    , font(fontId)
    , text(text)
    , color(color)
    , t(_life)
    , borderColor(_borderColor)
    , hasBorder(true)
    , width(_width)
{

}

void FadingTextParticle::Draw(Graph* gui)
{
    color.a = static_cast<Uint8>(255 * t.RemainingPart());
    if (width == 0)
    {
        if (hasBorder)
        {
            gui->WriteBorderedText(*font, text, x, y, color, borderColor);
        }
        else
        {
            gui->WriteNormal(*font, text, (int)x, (int)y, color);
        }
    }
    else
    {
        gui->WriteParagraph(*font, text, (int)x, (int)y, width, 10, color);
    }
}

FadingOutPointerParticle::FadingOutPointerParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, int _time, GLfloat maxDx, GLfloat maxDy)
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
        dx = (maxDx > 0) ? 1.0f : -1.0f;
    }

    if (maxDy != 0)
    {
        dy = (maxDy > 0) ? 1.0f : -1.0f;
    }
}

void FadingOutPointerParticle::Draw(Graph* gui)
{
    gui->PushAlpha((GLfloat)t.RemainingPart());
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

TraceSilhouetteParticle::TraceSilhouetteParticle(sprite_id _texture,
                                                 SDL_Rect& _coordinateRect,
                                                 SDL_Rect& _texRect,
                                                 int _life,
                                                 int _time,
                                                 SDL_RendererFlip _flip)
    : Particle(_texture, (GLfloat)coordinateRect.x, (GLfloat)coordinateRect.y, _life, _time, nullptr)
    , coordinateRect(_coordinateRect)
    , texRect(_texRect)
    , flip(_flip)
{

}

void TraceSilhouetteParticle::Draw(Graph* g)
{
    g->PushAlpha(0.5f * (1 - (GLfloat)lives / lifetime));
    g->DrawTexture(&coordinateRect, texture, &texRect, 0, flip);
    g->PopAlpha();
}


TargetedMovingParticle::TargetedMovingParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _time, MovementNodeCollection& _nodes, GraphColor _color)
    : Particle(_texture, _x, _y, 100, _time)
    , nodes(_nodes)
    , color(_color)
    , useTexture(true)
    , currentStartTime(_time)
    , currentNode(0)
    , startx(_x)
    , starty(_y)
{

}

TargetedMovingParticle::TargetedMovingParticle(GLfloat _x, GLfloat _y, int _time, MovementNodeCollection& _nodes, GraphColor _color, size_t _w, size_t _h)
    : Particle(0, _x, _y, 100, _time)
    , nodes(_nodes)
    , color(_color)
    , useTexture(false)
    , currentStartTime(_time)
    , currentNode(0)
    , startx(_x)
    , starty(_y)
    , w(_w)
    , h(_h)
{

}

void TargetedMovingParticle::Update(int new_time)
{
    if (currentNode >= nodes.size())
    {
        dead = true;
        return;
    }

    float t = (new_time - currentStartTime) / (float)nodes[currentNode].moveTime;

    if (t >= 1)
    {
        x = nodes[currentNode].targetX;
        y = nodes[currentNode].targetY;
        startx = x;
        starty = y;
        currentNode += 1;
        currentStartTime = new_time;
    }
    else
    {
        if (nodes[currentNode].fadeOut)
        {
            color.a = (1 - t);
        }
        else
        {
            color.a = 1.0f;
        }

        t = t * t * (3 - 2 * t);
        t = t * t * t;

        x = startx + (t * (nodes[currentNode].targetX - startx));
        y = starty + (t * (nodes[currentNode].targetY - starty));
    }
}

void TargetedMovingParticle::Draw(Graph* g)
{
    if (useTexture)
    {
        g->PushAlpha(color.a);
        g->PushTextureColorValues(color);
        g->DrawTexture(x, y, texture);
        g->PopTextureColorValue();
        g->PopAlpha();
    }
    else
    {
        g->DrawRect((int)x, (int)y, w, h, color);
    }
}

ConfigurableTextParticle::ConfigurableTextParticle(TextParticleConfig& c, std::string text)
    : MovingTextParticle(c.x, c.y, c.life, c.dx, c.dy, c.time, c.fontId, text, c.color, c.borderColor)
    , fadeOut(c.fadeOut)
    , currentScale(c.scaling ? c.maxScale : 1.0f)
    , scaling(c.scaling)
    , maxScale(c.maxScale)
    , minScale(c.minScale)
    , shake(c.shake)
    , shakeDeltaX(c.shakeX)
    , shakeDeltaY(c.shakeY)
    , intensity(c.intensity)
{

}

void ConfigurableTextParticle::Draw(Graph* gui)
{
    GLfloat t = (GLfloat)lives / lifetime - 1.0f;

    /*
    t = t * t * (3 - 2 * t);
    t = t * t * t;
    */
    t = (t * t * t * t * t) + 1.0f;

    if (scaling)
    {
        currentScale = minScale + (maxScale - minScale) * t;
    }

    if (fadeOut)
    {
        color.a = (Uint8)(255 * t);
    }

    GLfloat currentX = x;

    if (shake)
    {
        currentX += shakeDeltaX * (GLfloat)sin(t * 6.28 * intensity);
    }

    if (hasBorder)
    {
        gui->WriteBorderedText(*font, text, currentX, y, color, borderColor, currentScale);
    }
    else
    {
        gui->WriteNormal(*font, text, (int)currentX, (int)y, color, currentScale);
    }
    //gui->PopAlpha();
}

ConfigurableMovingParticle::ConfigurableMovingParticle(GLfloat x, GLfloat y, size_t time, sprite_id texture, MovingParticleConfig& c)
    : MovingParticle(texture, x, y, c.life, c.dx, c.dy, time)
    , color(c.color)
    , fadeOut(c.fadeOut)
    , minAlpha(c.minAlpha)
    , maxAlpha(c.maxAlpha)
    , scaling(c.scaling)
    , minScale(c.minScale)
    , maxScale(c.maxScale)
    , currentScale(c.scaling ? c.maxScale : 1.0f)
    , shake(c.shake)
    , shakeX(c.shakeX)
    , shakeY(c.shakeY)
    , intensity(c.intensity)
{

}

void ConfigurableMovingParticle::Draw(Graph* g)
{
    GLfloat t = (GLfloat)lives / lifetime - 1.0f;

    t = (t * t * t * t * t) + 1.0f;

    if (scaling)
    {
        currentScale = minScale + (maxScale - minScale) * t;
    }

    if (fadeOut)
    {
        color.a = minAlpha + (maxAlpha - minAlpha) * t;
    }

    GLfloat currentX = x;
    if (shake)
    {
        currentX += shakeX * (GLfloat)sin(t * 6.28 * intensity);
    }

    SDL_Rect r{ (int)GetX(), (int)GetY(), 0, 0 };
    
    g->PushAlpha(color.a);
    g->PushTextureColorValues(color);
    g->DrawTexture(&r, GetTexture(), GetFrame(), 0, SDL_FLIP_NONE);
    g->PopTextureColorValue();
    g->PopAlpha();
}

LeafParticle::LeafParticle(sprite_id _texture, GLfloat _x, GLfloat _y, int _life, int _time, int lifeCycle, GLfloat speedX, GLfloat speedY)
    : Particle(_texture, _x, _y, _life, _time)
    , speedX(speedX)
    , speedY(speedY)
    , lifeCycle(lifeCycle)
{

}

void LeafParticle::Update(int new_time)
{
    int p = new_time % lifeCycle;
    GLfloat t;
    if (p > lifeCycle / 2)
    {
        t = (new_time % lifeCycle) / (GLfloat)lifeCycle;
    }
    else
    {
        t = 1.0f - ((new_time % lifeCycle) / (GLfloat)lifeCycle);
    }

    t = t * t * (3 - 2 * t);
    t = t * t * t;

    GLfloat dx = speedX * t;
    GLfloat dy = speedY;

    x += static_cast<int>(dx / 30.0f);
    y += static_cast<int>(dy / 30.0f);
    Particle::Update(new_time);
}

void LeafParticle::Draw(Graph* gui)
{
    SDL_Rect r{ (int)GetX(), (int)GetY(), 0, 0 };
    gui->DrawTexture(&r, GetTexture(), GetFrame(), 0, SDL_FLIP_NONE);
}