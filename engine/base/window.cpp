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

#include "window.h"
#include "sound.h"
#include <vector>
#include <algorithm>

namespace EngineWindow
{

    static ConfirmationChoice lastChoice = ConfirmationChoice::NONE;

    static std::vector<GameWindow*> windows;

	GameWindow::GameWindow(int x,
		int y,
		size_t w,
		size_t h,
		size_t borderWidth,
		size_t fontId,
		Graph& g,
		SDL_Color color,
		SDL_Color borderColor,
		bool addToWindowStack)
		: x(x)
		, y(y)
		, width(w)
		, height(h)
		, g(&g)
		, color(color)
		, borderColor(borderColor)
		, borderWidth(borderWidth)
		, mainfont(fontId)
		, fadeOutSprite(0)
		, fadeOutTime(0)
		, fadeOutStart(0)
		, fadingOut(false)
		, fadeInTime(0)
		, fadeInStart(0)
		, fadingIn(false)
		, deleteOnFadeout(false)
    {
        int tmp = 0;
        g.GetTextSize(fontId, "Test", &tmp, &mainfontHeight);
		particle_timer.Reset();
		if (addToWindowStack)
		{
			windows.push_back(this);
		}
    }

    GameWindow::~GameWindow()
    {
        auto addr = std::find(windows.begin(), windows.end(), this);
        //technically, this should always happen
        if (addr != windows.end())
        {
            windows.erase(addr);
        }
    }

    void GameWindow::Draw()
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
				fadeAlpha = static_cast<Uint8>(255 * static_cast<double>(particle_timer.GetTicks() - fadeOutStart) / static_cast<double>(fadeOutTime));
				//("%d %d %f", particle_timer.GetTicks(), fadeOutStart, fadeOutTime);
			}
			if (fadeOutSprite != 0)
			{
				g->PushAlpha(fadeAlpha);
				g->DrawTextureStretched(x, y, width, height, g->GetTexture(fadeOutSprite));
				g->PopAlpha();
			}
			//g->DrawTextureStretched(g->GetTexture(fadeOutSprite));
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
				double timeDiff = static_cast<double>(particle_timer.GetTicks() - fadeInStart);
				fadeAlpha = static_cast<Uint8>(255 * (1 -  timeDiff / static_cast<double>(fadeInTime)));
			}
			
			if (fadeInSprite != 0)
			{
				g->PushAlpha(fadeAlpha);
				g->DrawTextureStretched(x, y, width, height, g->GetTexture(fadeInSprite));
				g->PopAlpha();
			}
			//g->DrawTextureStretched(g->GetTexture(fadeInSprite));
		}
    }

	void GameWindow::StartDraw()
	{
		if (fadingIn)
		{
			if (particle_timer.GetTicks() - fadeInStart >= fadeInTime)
			{
				fadingIn = false;
				OnFadeIn();
				g->PushAlpha(255);
			}
			else
			{
				g->PushAlpha(static_cast<Uint8>(255 * static_cast<double>(particle_timer.GetTicks() - fadeInStart) / static_cast<double>(fadeInTime)));
			}
		}
		else if (fadingOut)
		{
			if (particle_timer.GetTicks() - fadeOutStart >= fadeOutTime)
			{
				fadingOut = false;
				OnFadeOut();
				g->PushAlpha(0);
			}
			else
			{
				g->PushAlpha(static_cast<Uint8>(255 * (1 - static_cast<double>(particle_timer.GetTicks() - fadeOutStart) / static_cast<double>(fadeOutTime))));
			}
		}
		else
		{
			g->PushAlpha(255);
		}
	}

	void GameWindow::EndDraw()
	{
		g->PopAlpha();
	}

    void GameWindow::Update(const SDL_Event& event)
    {
		if (fadingIn || fadingOut)
		{
			return; 
		}

        switch (event.type)
        {
        case SDL_QUIT:
            delete this;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                delete this;
                break;
            }
        }
    }

	void GameWindow::FadeOut(sprite_id fadeOutSprite, size_t fadeOutTime, bool deleteOnFadeout)
	{
		fadingOut = true;
		fadingIn = false;
		this->fadeOutSprite = fadeOutSprite;
		this->fadeOutTime = fadeOutTime;
		this->deleteOnFadeout = deleteOnFadeout;
		EngineSound::FadeOutMusic(static_cast<int>(fadeOutTime));
		fadeOutStart = particle_timer.GetTicks();
	}

	void GameWindow::OnFadeOut()
	{
		if (deleteOnFadeout)
		{
			SDL_Event sdlevent;
			sdlevent.type = SDL_QUIT;
			SDL_PushEvent(&sdlevent);
		}
	}

	void GameWindow::FadeIn(sprite_id fadeInSprite, size_t fadeInTime)
	{
		fadingIn = true;
		fadingOut = false;
		this->fadeInSprite = fadeInSprite;
		this->fadeInTime = fadeInTime;
		fadeInStart = particle_timer.GetTicks();
	}

	void GameWindow::OnFadeIn()
	{

	}

	int GameWindow::GetCurrentTime()
	{
		return particle_timer.GetTicks();
	}

    NotificationWindow::NotificationWindow(int x,
                                           int y,
                                           size_t w,
                                           size_t h,
                                           size_t borderWidth,
                                           Graph& g,
                                           SDL_Color color,
                                           SDL_Color borderColor,
                                           std::string message,
                                           SDL_Color textColor,
                                           size_t fontId)
        : GameWindow(x, y, w, h, borderWidth, fontId, g, color, borderColor, true)
        , message(message)
        , textColor(textColor)
        , fontBorder(10)
    {

    }

    NotificationWindow::NotificationWindow(int x,
                                           int y,
                                           size_t borderWidth,
                                           Graph& g,
                                           SDL_Color color,
                                           SDL_Color borderColor,
                                           size_t fontBorder,
                                           std::string message,
                                           SDL_Color textColor,
                                           size_t fontId)
        : GameWindow(x, y, 0, 0, borderWidth, fontId, g, color, borderColor, true)
        , message(message)
        , textColor(textColor)
        , fontBorder(fontBorder)
    {
        int newW;
        int newH;
        g.GetTextSize(fontId, message, &newW, &newH);

        width += fontBorder * 2 + newW;
        height += fontBorder * 2 + newH;
    }

    void NotificationWindow::Draw()
    {
        GameWindow::Draw();
        g->WriteParagraph(mainfont, message, x + fontBorder, y + height / 3, width - fontBorder * 2, fontBorder, textColor);
    }

    void NotificationWindow::Update(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            delete this;
            break;
        case SDL_KEYDOWN:
            delete this;
            break;
        }
    }



    BGNotificationWindow::BGNotificationWindow(int x,
        int y,
        size_t w,
        size_t h,
        size_t borderWidth,
        Graph& g,
        sprite_id bg,
        std::string message,
        SDL_Color textColor,
        size_t fontId)
        : GameWindow(x, y, w, h, borderWidth, fontId, g, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, true)
        , message(message)
        , textColor(textColor)
        , fontBorder(borderWidth)
        , bg(bg)
    {

    }

    BGNotificationWindow::BGNotificationWindow(int x,
        int y,
        Graph& g,
        sprite_id bg,
        size_t fontBorder,
        std::string message,
        SDL_Color textColor,
        size_t fontId)
        : GameWindow(x, y, 0, 0, 0, fontId, g, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, true)
        , message(message)
        , textColor(textColor)
        , fontBorder(fontBorder)
        , bg(bg)
    {
        int newW;
        int newH;
        g.GetTextSize(fontId, message, &newW, &newH);

        width += fontBorder * 2 + newW;
        height += fontBorder * 2 + newH;
    }

    void BGNotificationWindow::Draw()
    {
        g->DrawTextureStretched(x, y, width, height, g->GetTexture(bg));
        g->WriteParagraph(mainfont, message, x + fontBorder, y + height / 3, width - fontBorder * 2, fontBorder, textColor);
    }

    void BGNotificationWindow::Update(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            delete this;
            break;
        case SDL_KEYDOWN:
            delete this;
            break;
        }
    }

    ConfirmationWindow::ConfirmationWindow(int x,
                                           int y,
                                           size_t borderWidth,
                                           Graph& g,
                                           size_t fontBorder,
                                           std::string message,
                                           SDL_Color textColor,
                                           SDL_Color bgColor,
                                           SDL_Color borderColor,
                                           size_t fontId)
        : GameWindow(x, y, 0, 0, borderWidth ,fontId, g, bgColor, borderColor, true)
        , fontColor(textColor)
        , fontBorder(fontBorder)
        , message(message)
    {
        int newW;
        int newH;
        g.GetTextSize(fontId, message, &newW, &newH);
        int testW;
        g.GetTextSize(fontId, "OKCancel", &testW, &newH);
        testW += 40;
        if (testW > newW)
        {
            newW = testW;
        }
        width += fontBorder * 2 + newW;
        height += fontBorder * 2 + newH + 3 * mainfontHeight;
        g.GetTextSize(fontId, "Cancel", &newW, &cancelTextW);
    }

    void ConfirmationWindow::Draw()
    {
        GameWindow::Draw();
        g->WriteParagraph(mainfont, message, x + fontBorder, y + height / 3, width - fontBorder * 2, fontBorder, fontColor);
        g->WriteNormal(mainfont, "CANCEL", x + 20, y + height - mainfontHeight * 2, fontColor);
        g->WriteNormal(mainfont, "OK", x + cancelTextW + 40, y + height - mainfontHeight * 2, fontColor);
        
    }

    void ConfirmationWindow::Update(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            delete this;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                OnOK();
                delete this;
                break;
            case SDLK_RETURN:
                OnCancel();
                delete this;
                break;
            }
        }
    }

    void ConfirmationWindow::OnOK()
    {
        lastChoice = ConfirmationChoice::YES;
    }

    void ConfirmationWindow::OnCancel()
    {
        lastChoice = ConfirmationChoice::NO;
    }

    bool UpdateWindow(const SDL_Event& event)
    {
        if (windows.size() > 0)
        {
            windows.back()->Update(event);
            return true;
        }
        return false;
    }

    void DrawWindows(bool active_only)
    {
        if (active_only)
        {
            if (windows.size() > 0)
            {
                windows.back()->Draw();
            }
        }
        else
        {
            for (auto w : windows)
            {
                w->Draw();
            }
        }
    }

    void CloseAll()
    {

    }

    bool HasOpenWindows()
    {
        return windows.size() > 0;
    }

    void ResetLastChoice()
    {
        lastChoice = ConfirmationChoice::NONE;
    }

    ConfirmationChoice GetLastChoice()
    {
        return lastChoice;
    }
}
