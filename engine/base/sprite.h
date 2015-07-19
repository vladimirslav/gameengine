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

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "graph.h"
#include <unordered_map>

struct ANIM_INFO
{
    size_t rowOnSpriteSheet;
    size_t frameAmount;
    size_t frameTime;
};

typedef std::unordered_map<std::string, ANIM_INFO> animation_list;

enum DIRECTION
{
    DIRECTION_LEFT,
    DIRECTION_RIGHT
};

class Sprite
{
public:
    Sprite(Graph* g,
           const std::string& spriteName,
           DIRECTION direction,
           const animation_list* animations,
           size_t frameW,
           size_t frameH,
           size_t currentTime,
           const std::string& initialAnim);
    void Draw(Graph* g, int x, int y);
    bool Update(size_t newTime); //true on animation ended
    void SetDirection(DIRECTION newDirection);

    // return true if anim with new id exists
    bool SwitchAnim(const std::string& newAnim);
    const std::string& getCurrentAnimationName() const;

protected:
    std::string currentAnimName;

    sprite_id spriteBg;
    DIRECTION direction;
    const animation_list* animations;
    size_t frameW;
    size_t frameH;

    size_t lastFrameChangeTime;

    const ANIM_INFO* currentAnim;

    size_t frameNum;

};

#endif