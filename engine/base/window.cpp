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
		const FontDescriptor* fontId,
		Graph& g,
		SDL_Color color,
		SDL_Color borderColor,
		bool addToWindowStack)
		: UiObject(x, y, w, h, g, color, borderColor, borderWidth, fontId)
        , index(-1)
		, prevIndex(-1)
        , activeObject(nullptr)
    {
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

        g->ClearAlpha();
    }

    void GameWindow::Draw()
    {
		UiObject::Draw();
    }

    void GameWindow::Update(const SDL_Event& event)
    {
        UpdateComponents(event);
		if (fadeState != FadeState::NO_FADE)
		{
			return; 
		}

        switch (event.type)
        {
        case SDL_MOUSEMOTION:
        {
            index = -1;
            if (event.motion.x > x &&
                event.motion.x < x + static_cast<int>(width) &&
                event.motion.y > y &&
                event.motion.y < y + static_cast<int>(height))
            {
                g->SwitchCursor(CursorType::ARROW);
                for (size_t i = 0; i < objectList.size(); i++)
                {
                    if (objectList[i]->HasMouseOver())
                    {
                        activeObject = objectList[i];
                        g->SwitchCursor(CursorType::POINTER);
                        index = objectList[i]->getCustomId();
                        if (prevIndex != index)
                        {
							EventHandling::SendEvent(EventHandling::Event{WINDOW_EVENT_MENU_MOVE, nullptr});
                            prevIndex = index;
                        }
                        objectList[i]->FadeIn(FadeMode::FADE_TO_BG, 0, 200);
                        break;
                    }
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP:
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                for (size_t i = 0; i < objectList.size(); i++)
                {
                    if (objectList[i]->IsClicked())
                    {
                        if (objectList[i]->Click() == false)
                        {
                            SDL_Event sdlevent;
                            sdlevent.type = SDL_KEYDOWN;
                            sdlevent.key.keysym.sym = SDLK_RETURN;
                            SDL_PushEvent(&sdlevent);
                        }
                        break;
                    }
                }
            }
            break;
        }
        case SDL_QUIT:
            delete this;
            break;
        }
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
                                           const FontDescriptor* fontId)
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
                                           const FontDescriptor* fontId)
        : GameWindow(x, y, 0, 0, borderWidth, fontId, g, color, borderColor, true)
        , message(message)
        , textColor(textColor)
        , fontBorder(fontBorder)
    {
        int newW;
        int newH;
        g.GetTextSize(*fontId, message, &newW, &newH);

        width += fontBorder * 2 + newW;
        height += fontBorder * 2 + newH;
    }

    void NotificationWindow::Draw()
    {
        GameWindow::Draw();
        g->WriteParagraph(*mainfont, message, x + fontBorder, y + height / 3, width - fontBorder * 2, fontBorder, textColor);
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
        const FontDescriptor* fontId)
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
        const FontDescriptor* fontId)
        : GameWindow(x, y, 0, 0, 0, fontId, g, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, true)
        , message(message)
        , textColor(textColor)
        , fontBorder(fontBorder)
        , bg(bg)
    {
        int newW;
        int newH;
        g.GetTextSize(*fontId, message, &newW, &newH);

        width += fontBorder * 2 + newW;
        height += fontBorder * 2 + newH;
    }

    void BGNotificationWindow::Draw()
    {
        StartDraw();
        g->DrawTextureStretched(x, y, width, height, g->GetTexture(bg));
        GameWindow::Draw();
        g->WriteParagraph(*mainfont, message, x + fontBorder, y + height / 3, width - fontBorder * 2, fontBorder, textColor);
        EndDraw();
    }

    void BGNotificationWindow::Update(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONUP:
        case SDL_QUIT:
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
                                           const FontDescriptor* font)
        : GameWindow(x, y, 0, 0, borderWidth, font, g, bgColor, borderColor, true)
        , fontColor(textColor)
        , fontBorder(fontBorder)
        , message(message)
    {
        int newW;
        int newH;
        g.GetTextSize(*font, message, &newW, &newH);
        int testW;
        g.GetTextSize(*font, "OKCancel", &testW, &newH);
        testW += 40;
        if (testW > newW)
        {
            newW = testW;
        }
        width += fontBorder * 2 + newW;
        height += fontBorder * 2 + newH + 3 * font->height;
        g.GetTextSize(*font, "Cancel", &newW, &cancelTextW);
    }

    void ConfirmationWindow::Draw()
    {
        GameWindow::Draw();
        g->WriteParagraph(*mainfont, message, x + fontBorder, y + height / 3, width - fontBorder * 2, fontBorder, fontColor);
        g->WriteNormal(*mainfont, "CANCEL", x + 20, y + height - mainfont->height * 2, fontColor);
        g->WriteNormal(*mainfont, "OK", x + cancelTextW + 40, y + height - mainfont->height * 2, fontColor);
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
        if (yesEvent != nullptr)
        {
            EventHandling::SendEvent(*yesEvent);
        }
    }

    void ConfirmationWindow::OnCancel()
    {
        lastChoice = ConfirmationChoice::NO;
        if (noEvent != nullptr)
        {
            EventHandling::SendEvent(*noEvent);
        }
    }

    void ConfirmationWindow::SetYesEvent(EventHandling::Event* ev)
    {
        yesEvent = ev;
    }

    void ConfirmationWindow::SetNoEvent(EventHandling::Event* ev)
    {
        noEvent = ev;
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
