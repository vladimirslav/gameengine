#include "..\SDL2\include\SDL.h"
#include "..\SDL2\include\SDL_ttf.h"
#include "..\SDL2\include\SDL_image.h"
#include "..\SDL2\include\SDL_mixer.h"

#include "routines.h"

#include <string>

#ifndef __Graph_H__
#define __Graph_H__

extern const int NORMAL_FONT_SIZE;

class Graph
{
private:
    int w;
    int h;

    SDL_DisplayMode displayMode;
    SDL_Window* screen;

    SDL_Color bgColor;
    TTF_Font* headFont;
    TTF_Font* normalFont;

    SDL_Renderer* renderer;

    bool fontLoaded;

public:
    Graph(int w, int h, const std::string fontFile, const std::string caption);
    ~Graph();
    const int &GetWidth() const;
    const int &GetHeight() const;
    SDL_Texture* LoadTexture(std::string filename);
    SDL_Texture* LoadTextureAlphaPink(std::string filename);
    void SetBgColor(SDL_Color color);
    void Flip();
    void FillScreen(SDL_Color color);
    void ClrScr();
    void PutPixel(int x, int y, SDL_Color color);
    void WriteHeading(std::string, int x, int y);
    void WriteNormal(std::string, int x, int y);
    void WriteNormal(std::string, int x, int y, SDL_Color color);
    void FillRect(int x1, int y1, int x2, int y2, SDL_Color color);

    void DrawTexture(int x, int y, SDL_Texture* texture);
private:
    void WriteText(TTF_Font* f, std::string, int x, int y, SDL_Color color);

};

#endif
