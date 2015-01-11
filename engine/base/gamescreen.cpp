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

#include "gamescreen.h"
#include "sound.h"

GameScreen::GameScreen(Graph& g)
    : g(&g)
    , fadeOutSprite(0)
    , fadeOutTime(0)
    , fadeOutStart(0)
    , fadingOut(false)
    , fadeInTime(0)
    , fadeInStart(0)
    , fadingIn(false)
{
    particle_timer.Reset();
}

GameScreen* GameScreen::Process()
{
    return this;
}

void GameScreen::Draw()
{
    if (fadingOut)
    {
        Uint8 fadeAlpha = 255;
        if (particle_timer.GetTicks() - fadeOutStart >= fadeOutTime)
        {
            fadeAlpha = 255;
            if (fadingOut)
            {
                fadingOut = false;
                OnFadeOut();
            }
        }
        else
        {
			fadeAlpha *= static_cast<Uint8>(static_cast<double>(particle_timer.GetTicks() - fadeOutStart) / static_cast<double>(fadeOutTime));
            //("%d %d %f", particle_timer.GetTicks(), fadeOutStart, fadeOutTime);
        }
        g->PushAlpha(fadeAlpha);
        g->DrawTextureStretched(g->GetTexture(fadeOutSprite));
        g->PopAlpha();
    }

    if (fadingIn)
    {
        Uint8 fadeAlpha = 255;
        if (particle_timer.GetTicks() - fadeInStart >= fadeInTime)
        {
            fadeAlpha = 0;
            if (fadingIn)
            {
                fadingIn = false;
                OnFadeIn();
            }
        }
        else
        {
			fadeAlpha *= static_cast<Uint8>(1 - static_cast<double>(particle_timer.GetTicks() - fadeInStart) / static_cast<double>(fadeInTime));
        }
        g->PushAlpha(fadeAlpha);
        g->DrawTextureStretched(g->GetTexture(fadeInSprite));
        g->PopAlpha();
    }
    g->Flip();
}

size_t GameScreen::GetCurrentFont()
{
    return 0;
}

Graph* GameScreen::GetGraph()
{
    return g;
}

int GameScreen::GetCurrentTime()
{
    return particle_timer.GetTicks();
}

void GameScreen::FadeOut(sprite_id fadeOutSprite, size_t fadeOutTime)
{
    fadingOut = true;
    fadingIn = false;
    this->fadeOutSprite = fadeOutSprite;
    this->fadeOutTime = fadeOutTime;
    EngineSound::FadeOutMusic(static_cast<int>(fadeOutTime));
    fadeOutStart = particle_timer.GetTicks();
}

void GameScreen::OnFadeOut()
{

}

void GameScreen::FadeIn(sprite_id fadeInSprite, size_t fadeInTime)
{
    fadingIn = true;
    fadingOut = false;
    this->fadeInSprite = fadeInSprite;
    this->fadeInTime = fadeInTime;
    fadeInStart = particle_timer.GetTicks();
}

void GameScreen::OnFadeIn()
{

}
