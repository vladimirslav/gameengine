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

#include "sprite.h"

Sprite::Sprite(Graph* g,
               const std::string& spriteName,
               DIRECTION direction,
               const animation_list* animations,
               size_t frameW,
               size_t frameH,
               size_t currentTime,
               const std::string& initialAnim)
    : spriteBg(g->LoadTexture(spriteName))
    , animations(animations)
    , direction(direction)
    , frameW(frameW)
    , frameH(frameH)
    , frameNum(0)
    , lastFrameChangeTime(currentTime)
    , currentAnimName(initialAnim)
    , noRepeat(false)
{
    auto anim = animations->find(initialAnim);
    if (anim != animations->cend())
    {
        currentAnim = &anim->second;
    }
    else
    {
        currentAnim = &animations->cbegin()->second;
    }
}

void Sprite::Draw(Graph* g, int x, int y)
{
    SDL_Rect current;
    current.x = frameW * frameNum;
    current.y = currentAnim->rowOnSpriteSheet * frameH;
    current.w = frameW;
    current.h = frameH;

    g->DrawTexture(x, y, spriteBg, &current, 0 ,direction == DIRECTION_RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

bool Sprite::Update(size_t newTime)
{
    if (newTime > lastFrameChangeTime + currentAnim->frameTime)
    {
        lastFrameChangeTime = newTime;
        if (frameNum >= currentAnim->frameAmount - 1)
        {
            if (noRepeat == false)
            {
                frameNum = 0;
            }
            return true; //animation ended
        }
        else
        {
            frameNum++;
        }
    }

    return frameNum >= currentAnim->frameAmount - 1;
}

bool Sprite::SwitchAnim(const std::string& newAnim, bool noRepeat)
{
    auto anim = animations->find(newAnim);
    if (anim != animations->cend())
    {
        this->noRepeat = noRepeat;
        frameNum = 0;
        currentAnimName = newAnim;
        currentAnim = &anim->second;
        return true;
    }

    return false;
}

const std::string& Sprite::getCurrentAnimationName() const
{
    return currentAnimName;
}

void Sprite::SetDirection(DIRECTION newDirection)
{
    direction = newDirection;
}