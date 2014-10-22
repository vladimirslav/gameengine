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

#include "graph.h"
#include <vector>

const SDL_Color F_WHITE = { 255, 255, 255, 255 };
const SDL_Color F_RED = { 255, 0, 0, 0 };
const SDL_Color F_BLACK = { 0, 0, 0, 0 };

const SDL_Color SELF_WHITE = {255,255,255};

/*
 * Initialize the window, where all stuff will be drawn
 * @param _w is window width
 * @param _h is window height
 * @param fontFile - file with TTF font to be used
 * @param caption - window caption
*/
Graph::Graph(int _w, int _h, const std::string caption)
    : screen(NULL)
    , w(_w)
    , h(_h)
    , renderer(NULL)
{
    SDL_SetAssertionHandler(EngineRoutines::handler, NULL);

    SDL_assert_release(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    SDL_assert_release(TTF_Init() == 0);

    SDL_assert_release(SDL_GetDesktopDisplayMode(0, &displayMode) == 0);

    screen = SDL_CreateWindow(caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _w, _h, SDL_WINDOW_SHOWN);
    SDL_assert_release(screen != NULL);
    
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_assert_release(renderer != NULL);

    alphaValues.push(255);
}

Graph::~Graph()
{
    
    FreeTextures();
    FreeFonts();
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);

    SDL_Quit();
}

/* Load font, return handler id to it */
size_t Graph::LoadFont(const std::string& fileName, size_t size)
{
    TTF_Font* fnt = TTF_OpenFont(fileName.c_str(), size);
    SDL_assert_release(fnt != NULL);
    fonts.push_back(fnt);
    return fonts.size() - 1;
}

void Graph::FreeFonts()
{
    for (auto font : fonts)
    {
        TTF_CloseFont(font);
    }

    fonts.clear();
}

/*
 * Set Background color to be set when clearing the screen
 */
void Graph::SetBgColor(SDL_Color color)
{
    bgColor = color;
}

/*
 * Clear the screen
*/
void Graph::ClrScr()
{
    SetViewPort(0, 0, w, h);
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);
}

/*
 * Flip the buffer
*/
void Graph::Flip()
{
    SDL_RenderPresent(renderer);
}

/* 
 * Fill the screen with the given color
*/
void Graph::FillScreen(const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

/*
 * Put pixel into given location
 * @param x is the x coordinate of the pixel
 * @param y is the y coordinate of the pixel
 * @param color is the color in Uint32
*/
void Graph::PutPixel(int x, int y, const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, x, y);
}

const int &Graph::GetWidth() const
{
    return w;
}

const int &Graph::GetHeight() const
{
    return h;
}

/*
 * write the text on the screen
 */
void Graph::WriteText(TTF_Font* f, const std::string& str, int x, int y, const SDL_Color& color)
{
    SDL_Surface* message = TTF_RenderText_Blended(f, str.c_str(), color);
    SDL_assert_release(message != NULL);
    SDL_Texture* preparedMsg = SDL_CreateTextureFromSurface(renderer, message);
    SDL_assert_release(preparedMsg != NULL);
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    SDL_assert_release(SDL_QueryTexture(preparedMsg, NULL, NULL, &dstrect.w, &dstrect.h) == 0);
    SDL_assert_release(SDL_RenderCopy(renderer, preparedMsg, NULL, &dstrect) == 0);
    SDL_FreeSurface(message);
    SDL_DestroyTexture(preparedMsg);
}

void Graph::WriteNormal(size_t fontHandler, const std::string& str, int x, int y)
{
    WriteText(fonts[fontHandler], str, x, y, SELF_WHITE);
}

void Graph::WriteNormal(size_t fontHandler, const std::string& str, int x, int y, const SDL_Color& color)
{
    WriteText(fonts[fontHandler], str, x, y, color);
}

void Graph::WriteParagraph(font_id fontHandler, const std::string& str, int x, int y, int maxW, size_t allowedBarrier, const SDL_Color& color)
{
    size_t i = 0;
    size_t row = 0;
    if (str.size() > 0)
    {
        while (i < str.size() - 1)
        {
            size_t final_letter = str.size() - 1;
            size_t right_border = final_letter;
            size_t left_border = i;

            bool wrote = false;
            do
            {
                int w = 0;
                int h = 0;
                GetTextSize(fontHandler, std::string(str.begin() + i, str.begin() + final_letter + 1), &w, &h);
                if (w <= maxW)
                {
                    if (final_letter == str.size() - 1 || (maxW - w) <= static_cast<int>(allowedBarrier))
                    {
                        WriteNormal(fontHandler, std::string(str.begin() + i, str.begin() + final_letter + 1), x, y + row * h, color);
                        wrote = true;
                    }
                    else
                    {
                        left_border = final_letter;
                    }
                }
                else
                {
                    right_border = final_letter;
                }

                if (wrote == false)
                {
                    final_letter = (left_border + right_border) / 2;
                    if (final_letter >= str.size())
                    {
                        final_letter = str.size() - 1;
                    }
                }
            } while (wrote == false);

            i = final_letter + 1;
            row++;
        }
    }
}

void Graph::GetTextSize(font_id fontHandler, const std::string& str, int* w, int* h)
{
    SDL_assert_release(TTF_SizeText(fonts[fontHandler], str.c_str(), w, h) == 0);
}

void Graph::DrawTexture(int x, int y, SDL_Texture* texture)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_assert_release(SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h) == 0);
    SDL_assert_release(SDL_SetTextureAlphaMod(texture, alphaValues.top()) == 0);
    SDL_assert_release(SDL_RenderCopy(renderer, texture, NULL, &dest) == 0);
}

void Graph::DrawTexture(int x, int y, sprite_id texture)
{
    SDL_Texture* tex = GetTexture(texture);
    SDL_assert_release(tex);
    DrawTexture(x, y, tex);
}

void Graph::DrawTextureStretched(SDL_Texture* texture)
{
    SDL_assert_release(SDL_RenderCopy(renderer, texture, NULL, NULL) == 0);
}

void Graph::DrawTextureStretched(int x, int y, int w, int h, SDL_Texture* texture)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
    SDL_assert_release(SDL_SetTextureAlphaMod(texture, alphaValues.top()) == 0);
    SDL_assert_release(SDL_RenderCopy(renderer, texture, NULL, &dest) == 0);
}

void Graph::DrawTexture(int x, int y, SDL_Texture* texture, const SDL_Rect* texPart, const double angle, const SDL_RendererFlip flip)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (texPart != NULL)
    {
        dest.w = texPart->w;
        dest.h = texPart->h;
    }
    else
    {
        SDL_assert_release(SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h) == 0);
    }

    SDL_assert_release(SDL_SetTextureAlphaMod(texture, alphaValues.top()) == 0);
    SDL_assert_release(SDL_RenderCopyEx(renderer, texture, texPart, &dest, angle, NULL, flip) == 0);
}

void Graph::DrawTexture(const SDL_Rect* destRect, sprite_id texture, const SDL_Rect* texPart, const double angle, const SDL_RendererFlip flip)
{
    SDL_Texture* tex = GetTexture(texture);
    SDL_assert_release(tex);
    SDL_assert_release(SDL_SetTextureAlphaMod(tex, alphaValues.top()) == 0);
    SDL_assert_release(SDL_RenderCopyEx(renderer, tex, texPart, destRect, angle, NULL, flip) == 0);
}

void Graph::DrawTexture(int x, int y, sprite_id texture, const SDL_Rect* texPart, const double angle, const SDL_RendererFlip flip)
{
    SDL_Texture* tex = GetTexture(texture);
    SDL_assert_release(tex);
    DrawTexture(x, y, tex, texPart, angle, flip);
}

void Graph::FillRect(int x1, int y1, int x2, int y2, const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect;
    rect.h = y2 - y1;
    rect.w = x2 - x1;
    rect.x = x1;
    rect.y = y1;
    SDL_assert_release(SDL_RenderDrawRect(renderer, &rect) == 0);
}

SDL_Texture* Graph::GetTexture(sprite_id id) const
{
    if (sprites.size() > id)
    {
        return sprites[id];
    }
    else
    {
        return nullptr;
    }
}

void Graph::GetTextureSize(sprite_id id, size_t* w, size_t* h) const
{
    SDL_Texture* target = GetTexture(id);
    SDL_assert_release(target);

    SDL_Rect result;
    SDL_assert_release(SDL_QueryTexture(target, NULL, NULL, &result.w, &result.h) == 0);

    if (w != NULL)
    {
        *w = result.w;
    }

    if (h != NULL)
    {
        *h = result.h;
    }
}

sprite_id Graph::LoadTexture(std::string filename)
{

    if (preloadedSprites.count(filename) != 0)
    {
        return preloadedSprites.at(filename);
    }

    SDL_Texture* result = IMG_LoadTexture(renderer, filename.c_str());
    SDL_assert_release(result != NULL);
    sprites.push_back(result);
    return sprites.size() - 1;
}


sprite_id Graph::LoadTextureAlphaPink(std::string filename)
{

    if (preloadedSprites.count(filename) != 0)
    {
        return preloadedSprites.at(filename);
    }

    SDL_Surface* loaded_image = NULL;
    SDL_Surface* optimized_image = NULL;
    loaded_image = IMG_Load(filename.c_str());
    if (loaded_image != NULL) {
        optimized_image = SDL_ConvertSurface(loaded_image, loaded_image->format, 0);
        SDL_FreeSurface(loaded_image);

        SDL_assert_release(optimized_image != NULL);

        Uint32 colorkey = SDL_MapRGB(optimized_image->format, 0xFF, 0x00, 0xFF);
        SDL_SetColorKey(optimized_image, SDL_TRUE, colorkey);

    }

    SDL_Texture* prepared_texture = SDL_CreateTextureFromSurface(renderer, optimized_image);

    SDL_assert_release(prepared_texture != NULL);
    sprites.push_back(prepared_texture);
    return sprites.size() - 1;
}

void Graph::FreeTextures()
{
    for (auto texture : sprites)
    {
        SDL_DestroyTexture(texture);
    }
    sprites.clear();
    preloadedSprites.clear();
}

void Graph::ApplyFilter(int x, int y, size_t w, size_t h, SDL_Color& color)
{
    SDL_Rect target{ x, y, w, h };

    SDL_BlendMode currentBlend;
    SDL_GetRenderDrawBlendMode(renderer, &currentBlend);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &target);
    SDL_SetRenderDrawBlendMode(renderer, currentBlend);
}

void Graph::GrayScaleFilter(int x, int y, size_t w, size_t h)
{
    SDL_Color c{ 128, 128, 128, 192 };
    ApplyFilter(x, y, w, h, c);
    //SDL_RenderDrawRect(renderer, &target);
    /*
    Uint8 pixels;
    SDL_RenderReadPixels(renderer, &target, 0, pixels, 0);
    for (int i = 0; i != w; ++i)
    {
        
        for (int y = 0; y != SDL_GetVideoSurface()->h; ++y)
        {
            Uint8 r = 0;
            Uint8 g = 0;
            Uint8 b = 0;
            SDL_GetRGB(pixel, SDL_GetVideoSurface()->format, &r, &g, &b);
            SDL_RenderDrawPoint(renderer, i, y);
            r = g = b = ((r + g + b) / 3);
            putpixel(SDL_GetVideoSurface(), i, y, SDL_MapRGB(SDL_GetVideoSurface()->format, r, g, b));
        }
    }
    */
}

void Graph::DrawRect(int x, int y, size_t w, size_t h, const SDL_Color& color)
{
    SDL_Rect box{ x, y, w, h };
    SDL_BlendMode currentBlend;
    SDL_GetRenderDrawBlendMode(renderer, &currentBlend);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &box);
    SDL_SetRenderDrawBlendMode(renderer, currentBlend);
}

void Graph::SetViewPort(int x, int y, size_t w, size_t h)
{
    SDL_Rect viewPort;
    viewPort.x = x;
    viewPort.y = y;
    viewPort.w = w;
    viewPort.h = h;
    SDL_RenderSetViewport(renderer, &viewPort);
}

void Graph::DrawLine(int x1, int y1, int x2, int y2, const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Graph::PushAlpha(Uint8 new_alpha)
{
    alphaValues.push(new_alpha);
}

void Graph::PopAlpha()
{
    alphaValues.pop();
}