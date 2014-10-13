#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "graph.h"

namespace EngineWindow
{
    enum class ConfirmationChoice
    {
        YES,
        NO,
        NONE,
    };

    class GameWindow
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

        size_t fontId;
        int fontHeight;

    public:
        GameWindow(int x, 
                   int y, 
                   size_t w, 
                   size_t h, 
                   size_t borderWidth,
                   size_t fontId,
                   Graph& g, 
                   SDL_Color color, 
                   SDL_Color borderColor);
        ~GameWindow();

        virtual void Draw();
        virtual void Update(const SDL_Event& event);
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
                           size_t fontId);

        NotificationWindow(int x,
                           int y,
                           size_t borderWidth,
                           Graph& g,
                           SDL_Color color,
                           SDL_Color borderColor,
                           size_t fontBorder,
                           std::string message,
                           SDL_Color textColor,
                           size_t fontId);
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
            size_t fontId);

        BGNotificationWindow(int x,
            int y,
            Graph& g,
            sprite_id bg,
            size_t fontBorder,
            std::string message,
            SDL_Color textColor,
            size_t fontId);
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
                           size_t fontId);

        virtual void Draw();
        virtual void Update(const SDL_Event& event);
    };

    bool HasOpenWindows();
    void DrawWindows(bool active_only = true);
    bool UpdateWindow(const SDL_Event& event);
    void CloseAll();
    void ResetLastChoice();
    ConfirmationChoice GetLastChoice();
}

#endif