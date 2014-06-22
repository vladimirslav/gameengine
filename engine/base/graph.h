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
    std::vector<SDL_Texture*> sprites;
    TextureIdMap preloadedSprites;

public:
    Graph(int w, int h, const std::string fontFile, const std::string caption);
    ~Graph();
    const int &GetWidth() const;
    const int &GetHeight() const;
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
    void DrawTexture(int x, int y, SDL_Texture* texture, const SDL_Rect* texPart, const double angle, const SDL_RendererFlip flip);

    sprite_id LoadSprite(std::string name);
    sprite_id LoadTexture(std::string filename);
    sprite_id LoadTextureAlphaPink(std::string filename);

    SDL_Texture* GetTexture(sprite_id id);
    void GetTextureSize(sprite_id id, size_t* w, size_t* h);
    void FreeTextures();

private:
    void WriteText(TTF_Font* f, std::string, int x, int y, SDL_Color color);

};

#endif
