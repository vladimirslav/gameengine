#ifndef __UIOBJECT_H__
#define __UIOBJECT_H__

#include "graph.h"
#include "countdown.h"

#include <vector>

class UiButton;

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

typedef void (*callback)();

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

    std::vector<UiObject*> objectList;

    bool mouseOver;
    bool isClicked;

    bool isHidden;

    int customId;

    callback onClick;

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
    ~UiObject();

    void SetX(int nx);
    void SetY(int ny);

	// Leave sprite as zero if FADE_TO_BG selected
    virtual void FadeIn(FadeMode mode, sprite_id fadeInSprite, size_t fadeInTime);
	virtual void OnFadeIn();
    virtual void FadeOut(FadeMode mode, sprite_id fadeOutSprite, size_t fadeOutTime, bool deleteOnFadeout = false);
	virtual void OnFadeOut();

	virtual void StartDraw();
	virtual void Draw();
	virtual void EndDraw();

    virtual void AddObject(UiObject *button);
    virtual void AddObject(UiObject *button, int x, int y);
    virtual void AddObject(UiObject *button, int x, int y, int customId);
    virtual void DrawObjects();
    virtual void ResetObjects();

    virtual void Update(const SDL_Event& event);
    virtual bool HasMouseOver();
    virtual bool IsClicked();

    virtual void setCustomId(int newId);
    virtual int  getCustomId();

    virtual void UpdateComponents(const SDL_Event& event);

    virtual bool IsHidden();
    virtual void Show();
    virtual void Hide();

    virtual void SetCallback(callback clickCallback);
    virtual bool Click();
};

#endif