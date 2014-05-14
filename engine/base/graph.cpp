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
    , fontLoaded(false)
{

    SDL_SetAssertionHandler(EngineRoutines::handler, NULL);

    SDL_assert_release(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    SDL_assert_release(TTF_Init() == 0);

    SDL_assert_release(SDL_GetDesktopDisplayMode(0, &displayMode) == 0);

    screen = SDL_CreateWindow(caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _w, _h, SDL_WINDOW_SHOWN);
    SDL_assert_release(screen != NULL);
    
    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_assert_release(renderer != NULL);

    if (fontFile.length() > 0)
    {
        headFont = TTF_OpenFont(fontFile.c_str(), 64);
        SDL_assert_release(headFont != NULL);
        normalFont = TTF_OpenFont(fontFile.c_str(), NORMAL_FONT_SIZE);
        SDL_assert_release(normalFont != NULL);
        fontLoaded = true;
    }
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
void Graph::FillScreen(SDL_Color color)
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
void Graph::PutPixel(int x, int y, SDL_Color color)
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
void Graph::WriteText(TTF_Font* f, std::string text, int x, int y, SDL_Color color)
{
    SDL_Surface* message = TTF_RenderText_Blended(f, text.c_str(), color);
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

void Graph::DrawTexture(int x, int y, SDL_Texture* texture)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_assert_release(SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h) == 0);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void Graph::FillRect(int x1, int y1, int x2, int y2, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect;
    rect.h = y2 - y1;
    rect.w = x2 - x1;
    rect.x = x1;
    rect.y = y1;
    SDL_assert_release(SDL_RenderDrawRect(renderer, &rect) == 0);
}


SDL_Texture* Graph::LoadTexture(std::string filename)
{
    SDL_Texture* result = IMG_LoadTexture(renderer, filename.c_str());
    SDL_assert_release(result != NULL);
    return result;
}


SDL_Texture* Graph::LoadTextureAlphaPink(std::string filename)
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
