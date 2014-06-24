#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "routines.h"

#include "..\SDL2\include\SDL.h"
#include "..\SDL2\include\SDL_ttf.h"
#include "..\SDL2\include\SDL_image.h"
#include "..\SDL2\include\SDL_mixer.h"

#include <string>
#include <vector>
#include <unordered_map>

typedef unsigned int sprite_id;
typedef std::unordered_map<std::string, sprite_id> TextureIdMap;
typedef std::vector<TTF_Font*> FontList;

class Graph
{
private:
    int w;
    int h;

    SDL_DisplayMode displayMode;
    SDL_Window* screen;

    SDL_Color bgColor;

    SDL_Renderer* renderer;

    std::vector<SDL_Texture*> sprites;
    TextureIdMap preloadedSprites;
    FontList fonts;

public:
    Graph(int w, int h, const std::string caption);
    ~Graph();
    const int &GetWidth() const;
    const int &GetHeight() const;
    void SetBgColor(SDL_Color color);
    void Flip();
    void FillScreen(SDL_Color color);
    void ClrScr();
    void PutPixel(int x, int y, SDL_Color color);
    void WriteNormal(size_t fontHandler, const std::string& str, int x, int y);
    void WriteNormal(size_t fontHandler, const std::string& str, int x, int y, SDL_Color color);
    void FillRect(int x1, int y1, int x2, int y2, SDL_Color color);

    void DrawTexture(int x, int y, SDL_Texture* texture);
    void DrawTexture(int x, int y, SDL_Texture* texture, const SDL_Rect* texPart, const double angle, const SDL_RendererFlip flip);

    void DrawTextureStretched(SDL_Texture* texture); //fullscreen
    void DrawTextureStretched(int x, int y, int w, int h, SDL_Texture* texture); //fixed width

    sprite_id LoadSprite(std::string name);
    sprite_id LoadTexture(std::string filename);
    sprite_id LoadTextureAlphaPink(std::string filename);

    SDL_Texture* GetTexture(sprite_id id);
    void GetTextureSize(sprite_id id, size_t* w, size_t* h);
    void FreeTextures();

    size_t LoadFont(const std::string& fileName, size_t size);
    void FreeFonts();

    void GrayScaleFilter(int x, int y, size_t w, size_t h);
    void DrawRect(int x, int y, size_t w, size_t h, SDL_Color color);

private:
    void WriteText(TTF_Font* f, const std::string& str, int x, int y, SDL_Color color);

};

#endif
