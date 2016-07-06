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
#include "countdown.h"
#include <vector>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty\stb_image.h"

//#define GL_GLEXT_PROTOTYPES
//#include <gl/GL.h>

const SDL_Color F_WHITE = { 255, 255, 255, 255 };
const SDL_Color F_RED = { 255, 0, 0, 0 };
const SDL_Color F_BLACK = { 0, 0, 0, 0 };

const SDL_Color SELF_WHITE = { 255, 255, 255, 255};

static const std::string SHAKE_TIMER = "shakeTimerScreen";

/*
 * Initialize the window, where all stuff will be drawn
 * @param _w is window width
 * @param _h is window height
 * @param fontFile - file with TTF font to be used
 * @param caption - window caption
*/
Graph::Graph(int _w, int _h, int _screen_w, int _screen_h, const std::string& caption)
    : w(_w)
    , h(_h)
	, screenW(_screen_w)
	, screenH(_screen_h)
	, shakeDeltaX(0)
	, shakeDeltaY(0)
    ,  BLACK(SDL_Color{ 0, 0, 0, 0 })
    , cursor(nullptr)
    , currentCursorType(CursorType::ARROW)
	, isFullScreen(false)
    , vertexAmount(0)
    , orthoLeft(0)
    , orthoTop(0)
    , orthoRight((GLfloat)w)
    , orthoBottom((GLfloat)h)
    , frameBuffer(0)
    , recheckWH(false)
    , postProcFlip(SDL_FLIP_VERTICAL)
    , prevX(0)
    , prevY(0)
{
    SDL_SetAssertionHandler(EngineRoutines::handler, NULL);

    SDL_assert_release(SDL_Init(SDL_INIT_EVERYTHING) == 0);
    SDL_assert_release(TTF_Init() == 0);

    SDL_assert_release(SDL_GetDesktopDisplayMode(0, &displayMode) == 0);

    screen = SDL_CreateWindow(caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, SDL_WINDOW_OPENGL);
    SDL_assert_release(screen != NULL);

    context = SDL_GL_CreateContext(screen);
    SDL_assert_release(context != NULL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
            "An error has occured",
            (const char*)glewGetErrorString(err),
            NULL);
    }

    alphaValues.push(1.0f);
    textureColorValues.push(GraphColor{ 1.0f, 1.0f, 1.0f, 1.0f });

    cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    SDL_SetCursor(cursor);

    shapeProgramId = LoadShaders("effects/baseshapev.glsl", "effects/baseshapef.glsl");
    textureProgramId = LoadShaders("effects/texv.glsl", "effects/texf.glsl");
    outlineProgramId = LoadShaders("effects/texv.glsl", "effects/outline.glsl");

    scenePostProcessingShader = LoadShaders("effects/scenev.glsl", "effects/scenet.glsl");
    defaultSceneProcessingShader = scenePostProcessingShader;
    //scenePostProcessingShader = textureProgramId;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &texVertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texVertBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texVertBuffData), texVertBuffData, GL_DYNAMIC_DRAW);

    RegenFrameBuffer();
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};

    glDrawBuffers(1, DrawBuffers);
    SDL_assert_release(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

}

void Graph::RegenFrameBuffer()
{
    if (frameBuffer != 0)
    {
        glDeleteFramebuffers(1, &frameBuffer);
    }

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    frameBufferTexture.h = screenH;
    frameBufferTexture.w = screenW;

    glGenTextures(1, &frameBufferTexture.texId);
    glBindTexture(GL_TEXTURE_2D, frameBufferTexture.texId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenW, screenH, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameBufferTexture.texId, 0);
    PrepareScreen();
}

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
};

void Graph::PrepareScreen()
{

    GLfloat ratioLogical = (GLfloat)w / (GLfloat)h;
    GLfloat ratioActual = (GLfloat)screenW / (GLfloat)screenH;

    orthoTop = 0;
    orthoBottom = (float)h;

    orthoLeft = 0;
    orthoRight = (float)w;

    if (std::abs(ratioActual - ratioLogical) > 0.01)
    {
        if (ratioActual > ratioLogical)
        {
            // screen is wider
            GLfloat marginX = (screenH * ratioLogical - screenW) / 2.0f;
            orthoLeft = marginX;
            orthoRight = w - marginX;
        }
        else
        {
            GLfloat marginY = (screenW * ratioActual - screenH) / 2.0f;
            orthoTop = marginY;
            orthoBottom = h - marginY;
        }
    }

    static const GLfloat BASE_ORTHO[16] = {
        2.f, 0.f, 0.f, 0.f,
        0.f, 2.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
       -1.f, 1.f, 0.f, 1.f,
    };
    memcpy(orthoProj, BASE_ORTHO, sizeof(BASE_ORTHO));
    // see here: https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
    orthoProj[0] /= (orthoRight - orthoLeft);
    orthoProj[5] /= (orthoTop - orthoBottom);

    orthoProj[12] = -(orthoRight + orthoLeft) / (orthoRight - orthoLeft);
    float oT = orthoTop;
    float oB = orthoBottom;
    orthoProj[13] = -(orthoTop + orthoBottom) / (orthoTop - orthoBottom);
}

GLfloat Graph::AdjustMouseX(int mx) const
{
    return orthoLeft + (( (GLfloat)mx / screenW ) * (orthoRight - orthoLeft));
}

GLfloat Graph::AdjustMouseY(int my) const
{
    return orthoTop + (( (GLfloat)my / screenH) * (orthoBottom - orthoTop));
}

void Graph::FlushTextures(GLuint texId, SDL_RendererFlip flip)
{
    FlushTextures(textureProgramId, texId, flip);
}

void Graph::FlushTextures(GLuint program, GLuint texId, SDL_RendererFlip flip, bool useCustomortho)
{
    glEnable(GL_TEXTURE_2D);
    glUseProgram(program);
    
    glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, orthoProj);
    glBindBuffer(GL_ARRAY_BUFFER, texVertBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, textureVertexAmount * sizeof(TexturedVertex), texVertBuffData);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedVertex),
        (void*)0
        );

    glActiveTexture(texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(texId, 0);

    GLfloat flips[2];
    flips[0] = (GLfloat)((flip & SDL_FLIP_HORIZONTAL) > 0 ? 1 : 0) * (texVertBuffData[0].u + texVertBuffData[4].u);
    flips[1] = (GLfloat)((flip & SDL_FLIP_VERTICAL) > 0 ? 1 : 0) * (texVertBuffData[0].v + texVertBuffData[4].v);
     
    glUniform2f(glGetUniformLocation(program, "flip"), flips[0], flips[1]);

    glUniform4f(glGetUniformLocation(program, "colorMod"),
                textureColorValues.top().r,
                textureColorValues.top().g,
                textureColorValues.top().b,
                alphaValues.top());

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedVertex),
        (void*)((char*)&texVertBuffData[0].u - (char*)texVertBuffData)
        );
    
    glDrawArrays(GL_TRIANGLES, 0, textureVertexAmount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    textureVertexAmount = 0;
    glDisable(GL_TEXTURE_2D);
    glUseProgram(0);
    //glBindTexture(GL_TEXTURE_2D, 0);
}

Graph::~Graph()
{

    FreeTextures();
    FreeFonts();
    TTF_Quit();

    glDeleteTextures(1, &frameBufferTexture.texId);
    if (frameBuffer != 0)
    {
        glDeleteFramebuffers(1, &frameBuffer);
    }
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &texVertBuffer);

    glDeleteProgram(textureProgramId);
    glDeleteProgram(outlineProgramId);
    glDeleteProgram(shapeProgramId);
    glDeleteProgram(scenePostProcessingShader);

    SDL_FreeCursor(cursor);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(screen);

    SDL_Quit();
}

/* Load font */
void Graph::LoadFontToDesc(FontDescriptor* desc)
{
    SDL_assert_release(desc != NULL);
    SDL_assert_release(desc->isLoaded == false);

    TTF_Font* fnt = TTF_OpenFont(desc->fontName.c_str(), desc->sizeToLoad);
    SDL_assert_release(fnt != NULL);

    fonts.push_back(fnt);

    desc->tableId = fonts.size() - 1;
    desc->isLoaded = true;

    if (desc->outline != 0)
    {
        TTF_SetFontOutline(fnt, desc->outline);
    }

    TTF_SizeText(fnt, "W", &desc->width, &desc->height);
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
    //SetViewPort(0, 0, w, h);
	int xdelta = 0; 
	int ydelta = 0;
	if (EngineTimer::IsActive(SHAKE_TIMER))
	{
		/*
		xdelta = shakeDeltaX - rand() % (shakeDeltaX * 2);
		ydelta = shakeDeltaY - rand() % (shakeDeltaY * 2);
		*/
        if (shakeDeltaX != 0)
        {
            xdelta = rand() % shakeDeltaX;
        }
		
        if (shakeDeltaY != 0)
        {
            ydelta = rand() % shakeDeltaY;
        }
	}

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, screenW, screenH);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);


    if (xdelta != 0 && ydelta != 0)
    {
        glTranslatef((GLfloat)xdelta, (GLfloat)ydelta, 0);
    }
}

/* set program to be used on post-processing */
void Graph::SetPostProcessingProgram(GLuint program, SDL_RendererFlip _postProcFlip)
{
    postProcFlip = _postProcFlip;
    scenePostProcessingShader = program;
}

void Graph::ResetPostprocessingProgram()
{
    postProcFlip = SDL_FLIP_VERTICAL;
    scenePostProcessingShader = defaultSceneProcessingShader;
}

void Graph::HideEdges()
{
    if ((size_t)(orthoRight - orthoLeft) != w)
    {
        if (orthoLeft < 0)
        {
            DrawRect((int)orthoLeft, (int)orthoTop, (size_t)-orthoLeft, (size_t)(orthoBottom - orthoTop), { 0, 0, 0, 1 });
        }
        if (orthoRight - w > 0)
        {
            DrawRect(w, (int)orthoTop, (size_t)(orthoRight - w), (size_t)(orthoBottom - orthoTop), { 0, 0, 0, 1 });
        }
    }
}

void Graph::ApplyShaderToScene(GLuint program)
{
    SDL_Rect destRect{ 0, 0, w, h };
    //glOrtho(0.0f, w, h, 0.0f, 0.0f, 1.0f);
    DrawTexture(program, &destRect, &frameBufferTexture, &destRect, 0, postProcFlip);
}

void Graph::FlushBuffer(GLuint shaderProgram, bool startNew)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    SDL_Rect destRect{ 0, 0, w, h };

    DrawScene(shaderProgram);
    if (startNew)
    {

        glClearColor(0, 0, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

/*
 * Flip the buffer
*/
void Graph::Flip()
{	
    if (EngineTimer::IsActive(SHAKE_TIMER))
    {
        if (useShakeFilter)
        {
            ApplyFilter(0, 0, w, h, shakeColor);
        }
    }

    FlushBuffer(scenePostProcessingShader, false);

    SDL_GL_SwapWindow(screen);

    if (recheckWH)
    {        
        RegenFrameBuffer();
        recheckWH = false;
    }
}

/*
 * Fill the screen with the given color
*/
void Graph::FillScreen(const SDL_Color& color)
{
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

/*
 * Put pixel into given location
 * @param x is the x coordinate of the pixel
 * @param y is the y coordinate of the pixel
 * @param color is the color in Uint32
*/
void Graph::PutPixel(int x, int y, const GraphColor& color)
{
    vertexBufferData[0] = Vertex((GLfloat)x, (GLfloat)y, 0);
    vertexAmount = 1;
    FlushBasicShape(color, GL_POINT);
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
void Graph::WriteText(TTF_Font* f, const std::string& str, int x, int y, const SDL_Color& color, GLfloat scale)
{
    if (str.empty())
    {
        return;
    }

    GLuint texture;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    SDL_Surface* message = TTF_RenderText_Blended(f, str.c_str(), color);
    SDL_assert_release(message != NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, message->w, message->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, message->pixels);

    texVertBuffData[0] = TexturedVertex((GLfloat)x, (GLfloat)y, 0.0f, 0.0f, 0.0f);
    texVertBuffData[1] = TexturedVertex((GLfloat)x, (GLfloat)y + message->h * scale, 0.0f, 0.0f, 1.0f);
    texVertBuffData[2] = TexturedVertex((GLfloat)x + message->w * scale, (GLfloat)y, 0.0f, 1.0f, 0.0f);

    texVertBuffData[3] = texVertBuffData[1];
    texVertBuffData[4] = TexturedVertex((GLfloat)x + message->w * scale, (GLfloat)y + message->h * scale, 0, 1.0f, 1.0f);
    texVertBuffData[5] = texVertBuffData[2];

    textureVertexAmount = 6;

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Since SDL text ignores alpha color value
    PushAlpha(color.a / 255.0f);
    FlushTextures(texture, SDL_FLIP_NONE);
    glDeleteTextures(1, &texture);
    SDL_FreeSurface(message);
    PopAlpha();
}

void Graph::WriteNormal(const FontDescriptor& fontHandler, const std::string& str, int x, int y)
{
    WriteText(fonts[fontHandler.tableId], str, x, y, SELF_WHITE);
}

void Graph::WriteNormal(const FontDescriptor& fontHandler, const std::string& str, int x, int y, const SDL_Color& color, GLfloat scale)
{
    WriteText(fonts[fontHandler.tableId], str, x, y, color, scale);
}

void Graph::WriteBorderedText(const FontDescriptor& fontHandler, 
    const std::string& str, 
    GLfloat x, 
    GLfloat y, 
    const SDL_Color& color, 
    SDL_Color borderColor,
    GLfloat scale)
{
    borderColor.a = color.a;
    //TODO: no need to make 5 surfaces (like it is done in WriteText), move the code back here to optimize the speed
    WriteText(fonts[fontHandler.tableId], str, (int)x - 1, (int)y, borderColor, scale);
    WriteText(fonts[fontHandler.tableId], str, (int)x + 1, (int)y, borderColor, scale);
    WriteText(fonts[fontHandler.tableId], str, (int)x, (int)y - 1, borderColor, scale);
    WriteText(fonts[fontHandler.tableId], str, (int)x, (int)y + 1, borderColor, scale);
    WriteText(fonts[fontHandler.tableId], str, (int)x, (int)y, color, scale);
}

void Graph::WriteBorderedParagraph(const FontDescriptor& fontHandler, const std::string& str, int x, int y, int maxW, size_t allowedBarrier, const SDL_Color& color, SDL_Color borderColor)
{
    borderColor.a = color.a;
    //TODO: no need to make 5 surfaces (like it is done in WriteText), move the code back here to optimize the speed
    WriteParagraph(fontHandler, str, x - 1, y, maxW, allowedBarrier, borderColor);
    WriteParagraph(fontHandler, str, x + 1, y, maxW, allowedBarrier, borderColor);
    WriteParagraph(fontHandler, str, x, y - 1, maxW, allowedBarrier, borderColor);
    WriteParagraph(fontHandler, str, x, y + 1, maxW, allowedBarrier, borderColor);

    WriteParagraph(fontHandler, str, x, y, maxW, allowedBarrier, color);
    /*
    WriteText(fonts[fontHandler.tableId], str, (int)x + 1, (int)y, borderColor, scale);
    WriteText(fonts[fontHandler.tableId], str, (int)x, (int)y - 1, borderColor, scale);
    WriteText(fonts[fontHandler.tableId], str, (int)x, (int)y + 1, borderColor, scale);
    WriteText(fonts[fontHandler.tableId], str, (int)x, (int)y, color, scale);
    */
}

void Graph::WriteParagraph(const FontDescriptor& fontHandler, const std::string& str, int x, int y, int maxW, size_t allowedBarrier, const SDL_Color& color)
{
    if (str.empty())
    {
        return;
    }
    SDL_Surface* message = TTF_RenderText_Blended_Wrapped(fonts[fontHandler.tableId], str.c_str(), color, maxW);
    SDL_assert_release(message != NULL);
    lastWrittenParagraphH = message->h;

    GLuint texture;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, message->w, message->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, message->pixels);

    texVertBuffData[0] = TexturedVertex((GLfloat)x, (GLfloat)y, 0.0f, 0.0f, 0.0f);
    texVertBuffData[1] = TexturedVertex((GLfloat)x, (GLfloat)y + message->h, 0.0f, 0.0f, 1.0f);
    texVertBuffData[2] = TexturedVertex((GLfloat)x + message->w, (GLfloat)y, 0.0f, 1.0f, 0.0f);

    texVertBuffData[3] = texVertBuffData[1];
    texVertBuffData[4] = TexturedVertex((GLfloat)x + message->w, (GLfloat)y + message->h, 0, 1.0f, 1.0f);
    texVertBuffData[5] = texVertBuffData[2];

    textureVertexAmount = 6;

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Since SDL text ignores alpha color value
    PushAlpha(color.a / 255.0f);

    FlushTextures(texture, SDL_FLIP_NONE);
    glDeleteTextures(1, &texture);
    SDL_FreeSurface(message);

    PopAlpha();
}

int Graph::GetLastWrittenParagraphH() const
{
    return lastWrittenParagraphH;
}

void Graph::GetTextSize(const FontDescriptor& fontHandler, const std::string& str, int* w, int* h)
{
    SDL_assert_release(TTF_SizeText(fonts[fontHandler.tableId], str.c_str(), w, h) == 0);
}

void Graph::FlushBasicShape(const GraphColor& color, GLenum mode)
{
    glUseProgram(shapeProgramId);
    glUniform4f(glGetUniformLocation(shapeProgramId, "colorval"), color.r, color.g, color.b, color.a);
    
    //glUniformMatrix4fv(glGetUniformLocation(shapeProgramId, "MVP"), 1, GL_FALSE, mtx);
    glUniformMatrix4fv(glGetUniformLocation(shapeProgramId, "MVP"), 1, GL_FALSE, orthoProj);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexAmount * sizeof(Vertex), vertexBufferData);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)0
        );

    glDrawArrays(mode, 0, vertexAmount);

    glDisableVertexAttribArray(0);
    vertexAmount = 0;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glUseProgram(0);
}

void Graph::DrawTexture(GLuint shaderProgramId, GLfloat x, GLfloat y, TextureRecord* texture)
{

    texVertBuffData[0] = TexturedVertex(x, y, 0.0f, 0.0f, 0.0f);
    texVertBuffData[1] = TexturedVertex(x, y + texture->h, 0.0f, 0.0f, 1.0f);
    texVertBuffData[2] = TexturedVertex(x + texture->w, y, 0.0f, 1.0f, 0.0f);

    texVertBuffData[3] = texVertBuffData[1];
    texVertBuffData[4] = TexturedVertex(x + texture->w, y + texture->h, 0, 1.0f, 1.0f);
    texVertBuffData[5] = texVertBuffData[2];
    //glUniform4f(glGetUniformLocation(shapeProgramId, "colorval"), color.r, color.g, color.b, color.a);

    textureVertexAmount = 6;

    FlushTextures(shaderProgramId, texture->texId, SDL_FLIP_NONE);
}

void Graph::DrawTexture(GLfloat x, GLfloat y, TextureRecord* texture)
{
    DrawTexture(textureProgramId, x, y, texture);
}

void Graph::DrawTexture(GLfloat x, GLfloat y, sprite_id texture)
{
    TextureRecord* tex = GetTexture(texture);
    SDL_assert_release(tex);
    DrawTexture(x, y, tex);
}

void Graph::DrawTextureStretched(TextureRecord* texture)
{
    texVertBuffData[0] = TexturedVertex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    texVertBuffData[1] = TexturedVertex(0.0f, (GLfloat)h, 0.0f, 0.0f, 1.0f);
    texVertBuffData[2] = TexturedVertex((GLfloat)w, 0.0f, 0.0f, 1.0f, 0.0f);

    texVertBuffData[3] = texVertBuffData[1];
    texVertBuffData[4] = TexturedVertex((GLfloat)w, (GLfloat)h, 0.0f, 1.0f, 1.0f);
    texVertBuffData[5] = texVertBuffData[2];
    //glUniform4f(glGetUniformLocation(shapeProgramId, "colorval"), color.r, color.g, color.b, color.a);

    textureVertexAmount = 6;

    FlushTextures(texture->texId, SDL_FLIP_NONE);
    /*
    SDL_assert_release(SDL_SetTextureColorMod(texture,
        textureColorValues.top().r,
        textureColorValues.top().g,
        textureColorValues.top().b) == 0);
    SDL_assert_release(SDL_SetTextureAlphaMod(texture, alphaValues.top()) == 0);
    SDL_assert_release(SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) == 0);
    SDL_assert_release(SDL_RenderCopy(renderer, texture, NULL, NULL) == 0);
    */
}

void Graph::DrawTextureStretched(GLfloat tx, GLfloat ty, GLfloat tw, GLfloat th, TextureRecord* texture)
{
    DrawTextureStretched(textureProgramId, tx, ty, tw, th, texture);
}

void Graph::DrawTextureStretched(GLuint shaderProgramId, GLfloat tx, GLfloat ty, GLfloat tw, GLfloat th, TextureRecord* texture)
{
    texVertBuffData[0] = TexturedVertex(tx, ty, 0.0f, 0.0f, 0.0f);
    texVertBuffData[1] = TexturedVertex(tx, ty + th, 0.0f, 0.0f, 1.0f);
    texVertBuffData[2] = TexturedVertex(tx + tw, ty, 0.0f, 1.0f, 0.0f);

    texVertBuffData[3] = texVertBuffData[1];
    texVertBuffData[4] = TexturedVertex(tx + tw, ty + th, 0, 1.0f, 1.0f);
    texVertBuffData[5] = texVertBuffData[2];

    textureVertexAmount = 6;

    FlushTextures(shaderProgramId, texture->texId, SDL_FLIP_NONE);
    /*
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
    SDL_assert_release(SDL_SetTextureAlphaMod(texture, alphaValues.top()) == 0);
    SDL_assert_release(SDL_SetTextureColorMod(texture,
        textureColorValues.top().r,
        textureColorValues.top().g,
        textureColorValues.top().b) == 0);
    SDL_assert_release(SDL_RenderCopy(renderer, texture, NULL, &dest) == 0);
    */
}

void Graph::DrawTexture(const SDL_Rect* destRect, sprite_id texture, const SDL_Rect* texPart, const double angle, const SDL_RendererFlip flip)
{
    Graph::DrawTexture(textureProgramId, destRect, texture, texPart, angle, flip);
}

void Graph::DrawTexture(GLuint shaderProgramId, const SDL_Rect* destRect, sprite_id texture, const SDL_Rect* texPart, const double angle, const SDL_RendererFlip flip)
{
    TextureRecord* tex = GetTexture(texture);
    SDL_assert_release(tex);
    SDL_assert_release(destRect);
    DrawTexture(shaderProgramId, destRect, tex, texPart, angle, flip);
}

void Graph::DrawTexture(GLuint shaderProgramId, const SDL_Rect* destRect, TextureRecord* tex, const SDL_Rect* texPart, const double angle, const SDL_RendererFlip flip)
{

    float ux = 0;
    float uy = 0;
    float uw = 1.0f;
    float uh = 1.0f;
    if (texPart != nullptr)
    {
        ux = texPart->x / (GLfloat)tex->w;
        uy = texPart->y / (GLfloat)tex->h;
        uw = texPart->w / (GLfloat)tex->w;
        uh = texPart->h / (GLfloat)tex->h;
    }

    float tx = (GLfloat)destRect->x;
    float ty = (GLfloat)destRect->y;

    float tw = (GLfloat)tex->w;
    float th = (GLfloat)tex->h;

    if (destRect->w)
    {
        tw = (GLfloat)destRect->w;
    }

    if (destRect->h)
    {
        th = (GLfloat)destRect->h;
    }

    texVertBuffData[0] = TexturedVertex(tx, ty, 0.0f, ux, uy);
    texVertBuffData[1] = TexturedVertex(tx, ty + th, 0.0f, ux, uy + uh);
    texVertBuffData[2] = TexturedVertex(tx + tw, ty, 0.0f, ux + uw, uy);

    texVertBuffData[3] = texVertBuffData[1];
    texVertBuffData[4] = TexturedVertex(tx + tw, ty + th, 0, ux + uw, uy + uh);
    texVertBuffData[5] = texVertBuffData[2];

    textureVertexAmount = 6;

    FlushTextures(shaderProgramId, tex->texId, flip);
}

void Graph::DrawScene(GLuint shaderProgramId)
{
    float tx = -1;
    float ty = -1;
    float tw = 2;
    float th = 2;

    float ux = 0;
    float uy = 0;
    float uh = 1;
    float uw = 1;

    texVertBuffData[0] = TexturedVertex(tx, ty, 0.0f, ux, uy);
    texVertBuffData[1] = TexturedVertex(tx, ty + th, 0.0f, ux, uy + uh);
    texVertBuffData[2] = TexturedVertex(tx + tw, ty, 0.0f, ux + uw, uy);

    texVertBuffData[3] = texVertBuffData[1];
    texVertBuffData[4] = TexturedVertex(tx + tw, ty + th, 0, ux + uw, uy + uh);
    texVertBuffData[5] = texVertBuffData[2];

    textureVertexAmount = 6;

    FlushTextures(shaderProgramId, frameBufferTexture.texId, SDL_FLIP_NONE, false);
}

TextureRecord* Graph::GetTexture(sprite_id id) const
{
    if (spriteList.size() > id)
    {
        return spriteList.at(id).get();
    }
    else
    {
        return nullptr;
    }
}

void Graph::GetTextureSize(sprite_id id, size_t* w, size_t* h) const
{
    TextureRecord* target = GetTexture(id);
    SDL_assert_release(target);

    if (w != NULL)
    {
        *w = target->w;
    }

    if (h != NULL)
    {
        *h = target->h;
    }
}

TextureRecord::TextureRecord()
{
    w = 0;
    h = 0;
    texId = 0;
}

TextureRecord::~TextureRecord()
{
    glDeleteTextures(1, &texId);
}

sprite_id Graph::LoadTexture(std::string filename)
{
    std::auto_ptr<TextureRecord> rec(new TextureRecord);

    if (preloadedSprites.count(filename) != 0)
    {
        return preloadedSprites.at(filename);
    }
    int gw;
    int gh;
    int cmp;
    unsigned char* img = stbi_load(filename.c_str(), &gw, &gh, &cmp, STBI_rgb_alpha);
    if (img == nullptr)
    {
        std::string err = "Could not load " + filename;
        EngineRoutines::ShowSimpleMsg(err.c_str());
    }
    else
    {
        rec->w = gw;
        rec->h = gh;
        glGenTextures(1, &rec->texId);
        glBindTexture(GL_TEXTURE_2D, rec->texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (cmp == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rec->w, rec->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rec->w, rec->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        }
        spriteList.push_back(std::move(rec));

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(img);
        // return spritesOpengl.size() - 1;
    }


    preloadedSprites[filename] = spriteList.size() - 1;
    return spriteList.size() - 1;
}

void Graph::ToggleFullscreen()
{
	if (isFullScreen)
	{
		isFullScreen = false;
        screenW = w;
        screenH = h;
        SDL_SetWindowSize(screen, screenW, screenH);
        SDL_SetWindowPosition(screen, prevX, prevY);
        SDL_SetWindowFullscreen(screen, 0);
        SDL_SetWindowBordered(screen, SDL_TRUE);
	}
	else
	{
        SDL_GetWindowPosition(screen, &prevX, &prevY);
		isFullScreen = true;
        SDL_DisplayMode dm;
        for (int i = 0; i < SDL_GetNumVideoDisplays(); i++)
        {
            SDL_assert_release(SDL_GetCurrentDisplayMode(i, &dm) == 0);
            if (dm.w > screenW)
            {
                screenW = dm.w;
                screenH = dm.h;
            }
        }

        SDL_SetWindowSize(screen, screenW, screenH);
        SDL_SetWindowPosition(screen, 0, 0);
        SDL_SetWindowBordered(screen, SDL_FALSE);
        SDL_SetWindowFullscreen(screen, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
    RegenFrameBuffer();
    recheckWH = true;
}

bool Graph::IsInFullScreen() const
{
	return isFullScreen;
}

void Graph::FreeTextures()
{
    spriteList.clear();
    preloadedSprites.clear();
}

void Graph::ApplyFilter(int x, int y, size_t w, size_t h, SDL_Color& color)
{
    GraphColor c{color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
    DrawRect(x, y, w, h, c);
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

void Graph::DrawRect(int nx, int ny, size_t w, size_t h, const GraphColor& color)
{
    GLfloat x = (GLfloat)nx;
    GLfloat y = (GLfloat)ny;

    vertexBufferData[0] = Vertex(x, y, 0);
    vertexBufferData[1] = Vertex(x, y + h, 0);
    vertexBufferData[2] = Vertex(x + w, y, 0);

    vertexBufferData[3] = vertexBufferData[1];
    vertexBufferData[4] = Vertex(x + w, y + h, 0);
    vertexBufferData[5] = vertexBufferData[2];

    vertexAmount = 6;
    FlushBasicShape(color, GL_TRIANGLES);
}

void Graph::SetViewPort(int x, int y, size_t w, size_t h)
{
    //glViewport(x, y, w, h);
}

void Graph::DrawBorders(int x, int y, size_t w, size_t h, size_t thickness, const GraphColor& color)
{
    for (size_t i = 0; i < thickness; i++)
    {
        DrawLine(x, y + i, x + w, y + i, color); // horizontal upper
        DrawLine(x, y + h - i, x + w, y + h - i, color); // horizontal lower

        DrawLine(x + i, y, x + i, y + h, color); // vertical left
        DrawLine(x + w - i, y, x + w - i, y + h, color); // vertical right
    }
}

void Graph::DrawLine(int x1, int y1, int x2, int y2, const GraphColor& color)
{
    vertexBufferData[0] = Vertex((GLfloat)x1, (GLfloat)y1, 0);
    vertexBufferData[1] = Vertex((GLfloat)x2, (GLfloat)y2, 0);
    vertexAmount = 2;
    FlushBasicShape(color, GL_LINES);
}

void Graph::PushAlpha(GLfloat new_alpha)
{
    alphaValues.push(new_alpha);
}

void Graph::PopAlpha()
{
    // 255 always stays
    if (alphaValues.size() > 1)
    {
        alphaValues.pop();
    }
}

void Graph::ClearAlpha()
{
    alphaValues = std::stack<GLfloat>();
    alphaValues.push(1.0f);
}

void Graph::PushTextureColorValues(Uint8 r, Uint8 g, Uint8 b)
{
    textureColorValues.push(GraphColor{ r / 255.0f, g / 255.0f, b / 255.0f, 0 });
}

void Graph::PushTextureColorValues(GraphColor& c)
{
    textureColorValues.push(c);
}

void Graph::PopTextureColorValue()
{
    textureColorValues.pop();
}


void Graph::HideCursor()
{
    SDL_ShowCursor(SDL_DISABLE);
}

void Graph::ShowCursor()
{
    SDL_ShowCursor(SDL_ENABLE);
}

void Graph::SetIcon(const std::string& icon_name)
{
    SDL_Surface* icon = SDL_LoadBMP(icon_name.c_str());
    SDL_SetWindowIcon(screen, icon);
    SDL_FreeSurface(icon);
}

void Graph::SetShake(size_t time, int deltax, int deltay)
{
    useShakeFilter = false;
	EngineTimer::StartTimer(SHAKE_TIMER, time);
	shakeDeltaX = deltax;
	shakeDeltaY = deltay;
}

void Graph::SetShake(size_t time, int deltax, int deltay, SDL_Color color)
{
    SetShake(time, deltax, deltay);
    shakeColor = color;
    useShakeFilter = true;
}

void StopShake()
{
	EngineTimer::StartTimer(SHAKE_TIMER, 0);
}

void Graph::SwitchCursor(CursorType type)
{
    if (currentCursorType != type)
    {
        currentCursorType = type;
        SDL_FreeCursor(cursor);
        if (type == CursorType::POINTER)
        {
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
        }
        else
        {
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
        }
        SDL_SetCursor(cursor);
    }
}