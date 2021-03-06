#include "uiobject.h"
#include "ui\uibutton.h"

UiObject::UiObject(int x,
				   int y,
				   size_t w,
				   size_t h,
				   Graph& g,
				   SDL_Color color,
				   SDL_Color borderColor,
				   size_t borderWidth,
				   const FontDescriptor* font)
	: x(x)
	, y(y)
	, width(w)
	, height(h)
	, g(&g)
	, color(color)
	, borderColor(borderColor)
	, borderWidth(borderWidth)
	, mainfont(font)
	, fadeSprite(0)
	, deleteOnFadeout(false)
	, fadeState(FadeState::NO_FADE)
	, fadeMode(FadeMode::SPRITE_FADE)
    , mouseOver(false)
    , isClicked(false)
    , isHidden(false)
    , customId(-1)
    , onClick(nullptr)
    , onTooltip(nullptr)
    , tooltip()
    , callbackParams(nullptr)
{
}

void UiObject::ResetStates()
{
    mouseOver = false;
    isClicked = false;
}

void UiObject::Highlight()
{
    isHighlighted = true;
}

void UiObject::DisableHighlight()
{
    isHighlighted = false;
}

void UiObject::StartDraw()
{
    if (flashingTime > 0)
    {
        if (flashCountdown.IsActive() == false)
        {
            flashCountdown.Reset(flashingTime);
        }
    }

    if (fadeMode == FadeMode::FADE_TO_BG)
    {
        switch (fadeState)
        {
        case FadeState::FADE_IN:
            g->PushAlpha((GLfloat)fadeCountdown.GonePart());
            break;
        case FadeState::FADE_OUT:
            g->PushAlpha((GLfloat)fadeCountdown.RemainingPart());
            break;
        default:
            break;
        }
    }
}

void UiObject::Flash(size_t ftime, GLuint flashProgram)
{
    flashingProgram = flashProgram;
    flashingTime = ftime;
    flashCountdown.Reset(ftime);
}

void UiObject::DisableFlash()
{
    flashingTime = 0;
    flashCountdown.Deactivate();
}

void UiObject::Draw()
{
    if (isHidden == false)
    {
        DrawObjects();
    }
}

void UiObject::Update(const SDL_Event& event)
{
    isClicked = false;
    if (event.type == SDL_MOUSEMOTION)
    {
        int mx = (int)g->AdjustMouseX(event.motion.x);
        int my = (int)g->AdjustMouseY(event.motion.y);
        if (g->AdjustMouseX(event.motion.x) >= x && g->AdjustMouseX(event.motion.x) <= x + static_cast<int>(width) &&
            g->AdjustMouseY(event.motion.y) >= y && g->AdjustMouseY(event.motion.y) <= y + static_cast<int>(height))
        {
            mouseOver = true;
        }
        else
        {
            mouseOver = false;
        }
    }
    else if (mouseOver && event.type == SDL_MOUSEBUTTONUP)
    {
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            isClicked = true;
            break;
        }
    }
}

void UiObject::UpdateComponents(const SDL_Event& event)
{
    for (auto but : objectList)
    {
        but->Update(event);
    }
}

bool UiObject::HasMouseOver()
{
    return mouseOver;
}

bool UiObject::IsClicked()
{
    return isClicked;
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

        if (fadeState != FadeState::NO_FADE)
        {
            g->PushAlpha(currentAlpha);
            g->DrawTextureStretched((GLfloat)x, (GLfloat)y, (GLfloat)width, (GLfloat)height, g->GetTexture(fadeSprite));
            g->PopAlpha();
        }
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

            // In case we set restarted fade countdown in one of the above functions
            if (fadeCountdown.IsActive() == false)
            {
                fadeState = FadeState::NO_FADE;
            }
        }
    }

    if (onTooltip != nullptr && mouseOver)
    {
        onTooltip(tooltip);
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

void UiObject::SetX(int nx)
{
    x = nx;
}

void UiObject::SetY(int ny)
{
    y = ny;
}

int UiObject::GetX() const
{
    return x;
}

int UiObject::GetY() const
{
    return y;
}

size_t UiObject::GetWidth() const
{
    return width;
}

size_t UiObject::GetHeight() const
{
    return height;
}

void UiObject::AddObject(UiObject *button)
{
    objectList.push_back(button);
}

void UiObject::AddObject(UiObject *button, int x, int y)
{
    button->SetX(this->x + x);
    button->SetY(this->y + y);
    objectList.push_back(button);
}

void UiObject::AddObject(UiObject *button, int x, int y, int customId)
{
    button->setCustomId(customId);
    this->AddObject(button, x, y);
}

void UiObject::ResetObjects()
{
    for (auto but : objectList)
    {
        delete but;
    }
    objectList.clear();
}

void UiObject::DrawObjects()
{
    for (auto but : objectList)
    {
        if (but->IsHidden() == false)
        {
            but->Draw();
        }
    }
}

UiObject::~UiObject()
{
    ResetObjects();
}

void UiObject::setCustomId(int newId)
{
    customId = newId;
}

int UiObject::getCustomId()
{
    return customId;
}

bool UiObject::IsHidden()
{
    return isHidden;
}

void UiObject::Show()
{
    isHidden = false;
}

void UiObject::Hide()
{
    isHidden = true;
}

void UiObject::SetCallback(callback clickCallback)
{
    onClick = clickCallback;
    callbackParams = nullptr;
}

void UiObject::SetCallback(callback clickCallback, void* callbackParams)
{
    onClick = clickCallback;
    this->callbackParams = callbackParams;
}

bool UiObject::Click()
{
    FadeIn(FadeMode::FADE_TO_BG, 0, 500);
    if (onClick != nullptr)
    {
        onClick(callbackParams);
        return true;
    }

    return false;
}

void UiObject::Receive(EventHandling::Event& e)
{

}

void UiObject::SetTooltipCallback(tooltipCallback tCallback, const char* tooltip)
{
    onTooltip = tCallback;
    this->tooltip = tooltip;
}

const FontDescriptor* UiObject::GetMainFont() const
{
    return mainfont;
}