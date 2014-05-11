#include "..\SDL2\include\SDL.h"
#include "..\SDL2\include\SDL_ttf.h"
#include "..\SDL2\include\SDL_image.h"
#include "..\SDL2\include\SDL_mixer.h"

#include "routines.h"

#include <string>

#ifndef __Graph_H__
#define __Graph_H__

extern const int NORMAL_FONT_SIZE;

extern const SDL_Color F_WHITE;
extern const SDL_Color F_BLACK;
extern const SDL_Color F_RED;

class Graph
{
private:
    int w;
    int h;

    SDL_DisplayMode displayMode;
    SDL_Window* screen;

    SDL_Surface* screenSurface;
    SDL_Texture* screenTexture;

    Uint32 bgColor;
    TTF_Font* headFont;
    TTF_Font* normalFont;

    SDL_Renderer* renderer;
    SDL_Rect surfaceRect;

public:
    Uint32 C_WHITE;
    Uint32 C_BLACK;
    Uint32 C_RED;

    SDL_Color F_WHITE;
    SDL_Color F_BLACK;
    SDL_Color F_RED;

public:
    Graph(int w, int h, const std::string fontFile, const std::string caption);
    ~Graph();
    const int &GetWidth() const;
    const int &GetHeight() const;
    SDL_Texture* LoadImage(std::string filename);
    void SetBgColor(Uint32 color);
    void Flip();
    void FillScreen(Uint32 color);
    void ClrScr();
    void PutPixel(int x, int y, Uint32 color);
    void WriteHeading(std::string, int x, int y);
    void WriteNormal(std::string, int x, int y);
    void WriteNormal(std::string, int x, int y, SDL_Color color);
    void FillRect(int x1, int y1, int x2, int y2, Uint32 color);
    void FillRect(int x1, int y1, int x2, int y2, int r, int g, int b, int alph);
    void DrawSprite(int x, int y, SDL_Surface* source, SDL_Rect* clip);
private:
    void WriteText(TTF_Font* f, std::string, int x, int y, SDL_Color color);
    void ApplySurface(int x, int y, SDL_Surface* source, SDL_Surface *dest, SDL_Rect* clip);
};

#endif
