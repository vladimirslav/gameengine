#ifndef __UIOBJECT_H__
#define __UIOBJECT_H__

#include "graph.h"
#include "countdown.h"

enum class FadeMode
{
	SPRITE_FADE, // fades into sprite or from sprite to original screen
	FADE_TO_BG,
};

enum class FadeState
{
	NO_FADE,
	FADE_IN,
	FADE_OUT
};

class UiObject
{
protected:

	int x;
	int y;
	size_t width;
	size_t height;
	Graph* g;
	SDL_Color color;
	SDL_Color borderColor;

	size_t borderWidth;

	size_t mainfont;
	int mainfontHeight;

	sprite_id fadeSprite;
	bool deleteOnFadeout;

	FadeState fadeState;
	FadeMode  fadeMode;

	EngineTimer::CountdownTimer fadeCountdown;

public:
	UiObject(int x,
			 int y,
			 size_t w,
			 size_t h,
			 Graph& g,
			 SDL_Color color,
			 SDL_Color borderColor,
			 size_t borderWidth,
			 size_t fontId);

	// Leave sprite as zero if FADE_TO_BG selected
    virtual void FadeIn(FadeMode mode, sprite_id fadeInSprite, size_t fadeInTime);
	virtual void OnFadeIn();
    virtual void FadeOut(FadeMode mode, sprite_id fadeOutSprite, size_t fadeOutTime, bool deleteOnFadeout = false);
	virtual void OnFadeOut();

	virtual void StartDraw();
	virtual void Draw();
	virtual void EndDraw();

};

#endif