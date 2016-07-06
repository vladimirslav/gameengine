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

#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "routines.h"

#include "..\SDL2\include\SDL.h"
#include "..\SDL2\include\SDL_ttf.h"
#include "..\SDL2\include\SDL_image.h"

#include "glew.h"
#include <SDL_opengl.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <stack>

typedef unsigned int sprite_id;

struct FontDescriptor
{
    size_t tableId;

    std::string fontName;
    int sizeToLoad;

    int width;
    int height;
    int outline;
    bool isLoaded;
};

typedef std::unordered_map<std::string, sprite_id> TextureIdMap;
typedef std::vector<TTF_Font*> FontList;

enum class CursorType
{
    ARROW,
    POINTER
};

struct TextureRecord
{
    int w;
    int h;
    GLuint texId;
    TextureRecord();
    ~TextureRecord();
};

struct Vertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;

    Vertex() : x(0.f), y(0.f), z(0.f) {}

    Vertex(GLfloat _x, GLfloat _y, GLfloat _z)
        : x(_x)
        , y(_y)
        , z(_z)
    {

    }
};

struct TexturedVertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat u;
    GLfloat v;

    TexturedVertex() : x(0.f), y(0.f), z(0.f), u(0.f), v(0.f) {}

    TexturedVertex(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _u, GLfloat _v)
        : x(_x)
        , y(_y)
        , z(_z)
        , u(_u)
        , v(_v)
    {

    }
};

struct GraphColor
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

class Graph
{
private:

    int w;
    int h;

    GLfloat orthoLeft;
    GLfloat orthoRight;
    
    GLfloat orthoTop;
    GLfloat orthoBottom;

	int screenW;
	int screenH;

    bool recheckWH;

    static const int MAX_BUFF_LEN = 12;
    int vertexAmount;
    Vertex vertexBufferData[MAX_BUFF_LEN]; // x y z
    GLuint vertexBuffer;

    int textureVertexAmount;
    TexturedVertex texVertBuffData[MAX_BUFF_LEN];
    GLuint texVertBuffer;


    SDL_Window* screen;
    SDL_DisplayMode displayMode;
    SDL_GLContext context;

    SDL_Color bgColor;

    std::vector<std::auto_ptr<TextureRecord>> spriteList;
    TextureIdMap preloadedSprites;
    FontList fonts;

    std::stack<GLfloat> alphaValues;
    std::stack<GraphColor> textureColorValues;

	int shakeDeltaX;
	int shakeDeltaY;
    bool useShakeFilter;
    SDL_Color shakeColor;

    SDL_Cursor* cursor;
    CursorType currentCursorType;
	bool isFullScreen;

    int lastWrittenParagraphH;
    void PrepareScreen();
    GLfloat orthoProj[16];

    GLuint shapeProgramId;
    GLuint textureProgramId;
    GLuint defaultSceneProcessingShader;
    GLuint scenePostProcessingShader;

    GLuint frameBuffer;
    TextureRecord frameBufferTexture;

    SDL_RendererFlip postProcFlip;
    int prevX;
    int prevY;

public:
    const SDL_Color BLACK;
    GLuint outlineProgramId;

    Graph(int _w, int _h, int _screen_w, int _screen_h, const std::string& caption);
    ~Graph();
    const int &GetWidth() const;
    const int &GetHeight() const;

    GLfloat AdjustMouseX(int mx) const;
    GLfloat AdjustMouseY(int my) const;

    void FlushTextures(GLuint texId, SDL_RendererFlip flip);
    void FlushTextures(GLuint program, GLuint texId, SDL_RendererFlip flip, bool useCustomOrtho = true);
    void FlushBasicShape(const GraphColor& color, GLenum mode);
    void ToggleFullscreen();
	bool IsInFullScreen() const;

    void SetIcon(const std::string& icon_name);
    void SetBgColor(SDL_Color color);
    void Flip();
    void FillScreen(const SDL_Color& color);
    void ClrScr();
    void PutPixel(int x, int y, const GraphColor& color);

    void GetTextSize(const FontDescriptor& fontHandler, const std::string& str, int* w, int* h);
    void WriteNormal(const FontDescriptor& fontHandler, const std::string& str, int x, int y);
    void WriteNormal(const FontDescriptor& fontHandler, const std::string& str, int x, int y, const SDL_Color& color, GLfloat scale = 1.0f);

    void SetPostProcessingProgram(GLuint program, SDL_RendererFlip flip);
    void ResetPostprocessingProgram();
    void HideEdges();

    void ApplyShaderToScene(GLuint program);
    void FlushBuffer(GLuint shaderProgram, bool startNew = false);

    void WriteBorderedText(const FontDescriptor& fontHandler,
                           const std::string& str,
                           GLfloat x,
                           GLfloat y,
                           const SDL_Color& color,
                           SDL_Color borderColor,
                           GLfloat scale = 1.0f);

    void WriteParagraph(const FontDescriptor& fontHandler, const std::string& str, int x, int y, int maxW, size_t allowedBarrier, const SDL_Color& color);
    void WriteBorderedParagraph(const FontDescriptor& fontHandler, const std::string& str, int x, int y, int maxW, size_t allowedBarrier, const SDL_Color& color, SDL_Color borderColor);

    int GetLastWrittenParagraphH() const;

    void DrawRect(int x, int y, size_t w, size_t h, const GraphColor& color);
    void DrawBorders(int x, int y, size_t w, size_t h, size_t thickness, const GraphColor& color);

    void DrawTexture(GLfloat x, GLfloat y, sprite_id texture);
    void DrawTexture(GLfloat x, GLfloat y, TextureRecord* texture);
    void DrawTexture(GLuint shaderProgramId, GLfloat x, GLfloat y, TextureRecord* texture);

    void DrawTexture(const SDL_Rect* destRect, sprite_id texture, const SDL_Rect* texPart, const double angle, const SDL_RendererFlip flip);
    void DrawTexture(GLuint shaderProgramId,
                     const SDL_Rect* destRect,
                     sprite_id texture,
                     const SDL_Rect* texPart,
                     const double angle,
                     const SDL_RendererFlip flip);
    void DrawTexture(GLuint shaderProgramId,
                    const SDL_Rect* destRect,
                    TextureRecord* texture,
                    const SDL_Rect* texPart,
                    const double angle,
                    const SDL_RendererFlip flip);

    void DrawScene(GLuint shaderProgramId);

    void DrawTextureStretched(TextureRecord* texture); //fullscreen
    void DrawTextureStretched(GLfloat tx, GLfloat ty, GLfloat tw, GLfloat th, TextureRecord* texture); //fixed width
    void DrawTextureStretched(GLuint shaderProgramId, GLfloat tx, GLfloat ty, GLfloat tw, GLfloat th, TextureRecord* texture); //fixed width

    sprite_id LoadTexture(std::string filename);

    TextureRecord* GetTexture(sprite_id id) const;
    void GetTextureSize(sprite_id id, size_t* w, size_t* h) const;
    void FreeTextures();

    void LoadFontToDesc(FontDescriptor* desc);

    void FreeFonts();

    void GrayScaleFilter(int x, int y, size_t w, size_t h);
    void ApplyFilter(int x, int y, size_t w, size_t h, SDL_Color& color);

    void DrawLine(int x1, int y1, int x2, int y2, const GraphColor& color);

    void SetViewPort(int x, int y, size_t w, size_t h);

    void PushAlpha(GLfloat new_alpha);
    void PopAlpha();
    void ClearAlpha(); // clear all alpha values

    void PushTextureColorValues(Uint8 r, Uint8 g, Uint8 b);
    void PushTextureColorValues(GraphColor& c);
    void PopTextureColorValue();

    void HideCursor();
    void ShowCursor();

	void SetShake(size_t time, int deltax, int deltay);
    void SetShake(size_t time, int deltax, int deltay, SDL_Color color);
	void StopShake();

    void SwitchCursor(CursorType type);

private:
    void WriteText(TTF_Font* f, const std::string& str, int x, int y, const SDL_Color& color, GLfloat scale = 1.0f);
    void RegenFrameBuffer();
};

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

#endif
