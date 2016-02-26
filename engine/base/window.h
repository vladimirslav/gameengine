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

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "graph.h"
#include "Timer.h"

#include "sound.h"
#include "uiobject.h"
#include "eventhandler.h"

namespace EngineWindow
{
	enum WINDOW_EVENT
	{
		WINDOW_EVENT_MENU_MOVE = 10000,
	};

    enum class ConfirmationChoice
    {
        YES,
        NO,
        NONE,
    };

    class GameWindow : public UiObject
    {
    protected:
		Timer particle_timer;
        int index;
		int prevIndex;
        UiObject* activeObject;
    public:
        GameWindow(int x,
                   int y,
                   size_t w,
                   size_t h,
                   size_t borderWidth,
                   const FontDescriptor* fontId,
                   Graph& g,
                   SDL_Color color,
                   SDL_Color borderColor,
				   bool addToWindowStack);
        virtual ~GameWindow();

        virtual void Draw();
        virtual void Update(const SDL_Event& event);

		virtual void OnFadeOut();

		virtual int GetCurrentTime();
    };

    class NotificationWindow : public GameWindow
    {
    protected:
        std::string message;
        SDL_Color textColor;
        int fontBorder;
    public:

        NotificationWindow(int x,
                           int y,
                           size_t w,
                           size_t h,
                           size_t borderWidth,
                           Graph& g,
                           SDL_Color color,
                           SDL_Color borderColor,
                           std::string message,
                           SDL_Color textColor,
                           const FontDescriptor* fontId);

        NotificationWindow(int x,
                           int y,
                           size_t borderWidth,
                           Graph& g,
                           SDL_Color color,
                           SDL_Color borderColor,
                           size_t fontBorder,
                           std::string message,
                           SDL_Color textColor,
                           const FontDescriptor* fontId);
        virtual void Draw();
        virtual void Update(const SDL_Event& event);
    };

    class BGNotificationWindow : public GameWindow
    {
    protected:
        std::string message;
        SDL_Color textColor;
        int fontBorder;
        sprite_id bg;
    public:

        BGNotificationWindow(int x,
            int y,
            size_t w,
            size_t h,
            size_t borderWidth,
            Graph& g,
            sprite_id bg,
            std::string message,
            SDL_Color textColor,
            const FontDescriptor* fontId);

        BGNotificationWindow(int x,
            int y,
            Graph& g,
            sprite_id bg,
            size_t fontBorder,
            std::string message,
            SDL_Color textColor,
            const FontDescriptor* fontId);
        virtual void Draw();
        virtual void Update(const SDL_Event& event);
    };

    class ConfirmationWindow : public GameWindow
    {
    protected:
        SDL_Color fontColor;
        size_t fontBorder;
        std::string message;
        int cancelTextW;

        virtual void OnOK();
        virtual void OnCancel();

        EventHandling::Event* yesEvent;
        EventHandling::Event* noEvent;

    public:

        ConfirmationWindow(int x,
                           int y,
                           size_t borderWidth,
                           Graph& g,
                           size_t fontBorder,
                           std::string message,
                           SDL_Color textColor,
                           SDL_Color bgColor,
                           SDL_Color borderColor,
                           const FontDescriptor* fontId);

        virtual void Draw();
        virtual void Update(const SDL_Event& event);
        virtual void SetYesEvent(EventHandling::Event* ev);
        virtual void SetNoEvent(EventHandling::Event* ev);
    };

    bool HasOpenWindows();
    void DrawWindows(bool active_only = true);
    bool UpdateWindow(const SDL_Event& event);
    void CloseAll();
    void ResetLastChoice();
    ConfirmationChoice GetLastChoice();
}

#endif
