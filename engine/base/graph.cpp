#include "graph.h"

const SDL_Color F_WHITE = { 255, 255, 255, 255 };
const SDL_Color F_RED = { 255, 0, 0, 0 };
const SDL_Color F_BLACK = { 0, 0, 0, 0 };

const SDL_Color SELF_WHITE = {255,255,255};

const int NORMAL_FONT_SIZE = 32;
/*
 * Initialize the window, where all stuff will be drawn
 * @param _w is window width
 * @param _h is window height
 * @param fontFile - file with TTF font to be used
 * @param caption - window caption
*/
Graph::Graph(int _w, int _h, const std::string fontFile, const std::string caption)
    : screen(NULL)
    , headFont(NULL)
    , normalFont(NULL)
    , w(_w)
    , h(_h)
    , renderer(NULL)
    , screenSurface(NULL)
    , screenTexture(NULL)
{

    surfaceRect.x = 0;
    surfaceRect.y = 0;
    surfaceRect.w = _w;
    surfaceRect.h = _h;

    SDL_SetAssertionHandler(EngineRoutines::handler, NULL);

    SDL_assert_release(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    SDL_assert_release(TTF_Init() == 0);

    SDL_assert_release(SDL_GetDesktopDisplayMode(0, &displayMode) == 0);

    screen = SDL_CreateWindow(caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _w, _h, SDL_WINDOW_SHOWN);
    SDL_assert_release(screen != NULL);
    
    renderer = SDL_CreateRenderer(screen, -1, 0);
    SDL_assert_release(renderer != NULL);

    screenSurface = SDL_CreateRGBSurface(0, _w, _h, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, _w, _h);

    bgColor = SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00);
    C_WHITE = SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF);
    C_BLACK = SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00);
    C_RED = SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00);
    headFont = TTF_OpenFont(fontFile.c_str(), 64);
    SDL_assert_release(headFont != NULL);
    normalFont = TTF_OpenFont(fontFile.c_str(), NORMAL_FONT_SIZE);
    SDL_assert_release(normalFont != NULL);
}

Graph::~Graph()
{
    TTF_CloseFont(headFont);
    TTF_CloseFont(normalFont);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);

    SDL_Quit();
}

void Graph::SetBgColor(Uint32 color)
{
    bgColor = color;
}

/*
 * Clear the screen
*/
void Graph::ClrScr()
{
    FillScreen(bgColor);
}

/*
 * Flip the buffer
*/
void Graph::Flip()
{
    SDL_UpdateTexture(screenTexture, NULL, screenSurface->pixels, screenSurface->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

/* 
 * Fill the screen with the given color
*/
void Graph::FillScreen(Uint32 color)
{
    SDL_FillRect(screenSurface, &screenSurface->clip_rect, color);
}

/*
 * Put pixel into given location
 * @param x is the x coordinate of the pixel
 * @param y is the y coordinate of the pixel
 * @param color is the color in Uint32
*/
void Graph::PutPixel(int x, int y, Uint32 color)
{
    if (x >= 0 && y >= 0 && x < w && y < h)
    {
        Uint8 *target_pixel = (Uint8 *)screenSurface->pixels + y * screenSurface->pitch + x * 4;
        *(Uint32 *)target_pixel = color;
    }
}

const int &Graph::GetWidth() const
{
    return w;
}

const int &Graph::GetHeight() const
{
    return h;
}

void Graph::WriteText(TTF_Font* f, std::string text, int x, int y, SDL_Color color)
{
    SDL_Surface *message = NULL;
    message = TTF_RenderText_Blended(f, text.c_str(), color);
    ApplySurface(x, y, message, screenSurface, NULL);
    SDL_FreeSurface(message);
}

void Graph::WriteHeading(std::string text, int x, int y)
{
    WriteText(headFont, text, x, y, SELF_WHITE);
}

void Graph::WriteNormal(std::string text, int x, int y)
{
    WriteText(normalFont, text, x, y, SELF_WHITE);
}

void Graph::WriteNormal(std::string text, int x, int y, SDL_Color color)
{
    WriteText(normalFont, text, x, y, color);
}

void Graph::DrawSprite( int x, int y, SDL_Surface* source, SDL_Rect* clip)
{ 
    ApplySurface(x, y, source, screenSurface, clip);
}

void Graph::ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{ 
    SDL_Rect offset; 
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset); 
}

void Graph::FillRect(int x1, int y1, int x2, int y2, Uint32 color)
{
    SDL_Rect r;
    r.h = y2 - y1;
    r.w = x2 - x1;
    r.x = x1;
    r.y = y1;
    SDL_FillRect(screenSurface, &r, color);
}

void Graph::FillRect(int x1, int y1, int x2, int y2, int r, int g, int b, int alpha)
{
    // Look into it, definitely can be optimized
    SDL_Rect rect;
    rect.h = y2 - y1;
    rect.w = x2 - x1;
    rect.x = x1;
    rect.y = y1;

    SDL_Rect rfill;
    rfill.h = y2 - y1;
    rfill.w = x2 - x1;
    rfill.x = 0;
    rfill.y = 0;
    SDL_Surface* s = SDL_ConvertSurfaceFormat(screenSurface, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_assert_release(s != NULL);
    SDL_assert_release(SDL_FillRect(s, &rfill, SDL_MapRGBA(screenSurface->format, r, g, b, alpha)));
    SDL_assert_release(SDL_SetSurfaceAlphaMod(s, alpha));
    ApplySurface(0, 0, s, screenSurface, &rect);
    SDL_FreeSurface(s);
}

SDL_Texture* Graph::LoadImage(std::string filename)
{
    SDL_Surface* loaded_image = NULL;
    SDL_Surface* optimized_image = NULL;
    loaded_image = IMG_Load(filename.c_str());
    if( loaded_image != NULL ) {
        optimized_image = SDL_ConvertSurface(loaded_image, loaded_image->format, 0);
        SDL_FreeSurface(loaded_image); 

        SDL_assert_release(optimized_image != NULL);

        Uint32 colorkey = SDL_MapRGB(optimized_image->format, 0xFF, 0x00, 0xFF);
        SDL_SetColorKey(optimized_image, SDL_TRUE, colorkey);

    }

    SDL_Texture* prepared_texture = SDL_CreateTextureFromSurface(renderer, optimized_image);

    SDL_assert_release(prepared_texture != NULL);
    return prepared_texture;
}
