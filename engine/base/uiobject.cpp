#include "uiobject.h"

UiObject::UiObject(int x,
				   int y,
				   size_t w,
				   size_t h,
				   Graph& g,
				   SDL_Color color,
				   SDL_Color borderColor,
				   size_t borderWidth,
				   size_t fontId)
	: x(x)
	, y(y)
	, width(w)
	, height(h)
	, g(&g)
	, color(color)
	, borderColor(borderColor)
	, borderWidth(borderWidth)
	, mainfont(fontId)
	, fadeSprite(0)
	, deleteOnFadeout(false)
	, fadeState(FadeState::NO_FADE)
	, fadeMode(FadeMode::SPRITE_FADE)
{
	int tmp = 0;
	g.GetTextSize(fontId, "Test", &tmp, &mainfontHeight);
}

void UiObject::StartDraw()
{
    if (fadeMode == FadeMode::FADE_TO_BG)
    {
        switch (fadeState)
        {
        case FadeState::FADE_IN:
            g->PushAlpha(static_cast<Uint8>(fadeCountdown.GonePart() * 255));
            break;
        case FadeState::FADE_OUT:
            g->PushAlpha(static_cast<Uint8>(fadeCountdown.RemainingPart() * 255));
            break;
        default:
            break;
        }
    }
}

void UiObject::Draw()
{
	//StartDraw();
	//EndDraw();
}

void UiObject::EndDraw()
{
    if (fadeMode == FadeMode::SPRITE_FADE)
    {
        Uint8 currentAlpha = 0;
        switch (fadeState)
        {
        case FadeState::FADE_IN:
            currentAlpha = static_cast<Uint8>(fadeCountdown.RemainingPart() * 255);
            break;
        case FadeState::FADE_OUT:
            currentAlpha = static_cast<Uint8>(fadeCountdown.GonePart() * 255);
            break;
        default:
            break;
        }

        g->PushAlpha(currentAlpha);
        g->DrawTextureStretched(x, y, width, height, g->GetTexture(fadeSprite));
        g->PopAlpha();
    }


    if (fadeState != FadeState::NO_FADE)
    {
        if (fadeMode == FadeMode::FADE_TO_BG)
        {
	        g->PopAlpha();
        }

        bool timerEnded = false;
        if (fadeCountdown.IsActive() == false)
        {
            if (fadeState == FadeState::FADE_IN)
            {
                OnFadeIn();
            }
            else if (fadeState == FadeState::FADE_OUT)
            {
                OnFadeOut();
            }
            fadeState = FadeState::NO_FADE;
        }
    }
}

void UiObject::FadeIn(FadeMode mode, sprite_id fadeInSprite, size_t fadeInTime)
{
	fadeState = FadeState::FADE_IN;
	fadeMode = mode;
	fadeCountdown.Reset(fadeInTime);
	fadeSprite = fadeInSprite;
}

void UiObject::OnFadeIn()
{

}

void UiObject::FadeOut(FadeMode mode,
                       sprite_id fadeOutSprite,
                       size_t fadeOutTime,
                       bool deleteOnFadeout)
{
    fadeState = FadeState::FADE_OUT;
    fadeMode = mode;
    fadeSprite = fadeOutSprite;
    fadeCountdown.Reset(fadeOutTime);
    this->deleteOnFadeout = deleteOnFadeout;
}

void UiObject::OnFadeOut()
{
}