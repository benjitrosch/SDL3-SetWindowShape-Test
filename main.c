#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL.h"
#include "SDL_main.h"

typedef int (*GLCREATEPROGRAMPROC)(void);
typedef int (*GLCREATESHADERPROC)(int);
typedef void (*GLSHADERSOURCEPROC)(int, int, const char **, const int *);
typedef void (*GLCOMPILESHADERPROC)(int);
typedef void (*GLATTACHSHADERPROC)(int, int);
typedef void (*GLLINKPROGRAMPROC)(int);
typedef void (*GLDELETESHADERPROC)(int);
typedef void (*GLUSEPROGRAMPROC)(int);
typedef void (*GLGENVERTEXARRAYSPROC)(int, int *);
typedef void (*GLBINDVERTEXARRAYPROC)(int);
typedef void (*GLCLEARPROC)(int);
typedef void (*GLCLEARCOLORPROC)(float, float, float, float);
typedef void (*GLDRAWARRAYSPROC)(int, int, int);

GLCREATEPROGRAMPROC   glCreateProgram;
GLCREATESHADERPROC    glCreateShader;
GLSHADERSOURCEPROC    glShaderSource;
GLCOMPILESHADERPROC   glCompileShader;
GLATTACHSHADERPROC    glAttachShader;
GLLINKPROGRAMPROC     glLinkProgram;
GLDELETESHADERPROC    glDeleteShader;
GLUSEPROGRAMPROC      glUseProgram;
GLGENVERTEXARRAYSPROC glGenVertexArrays;
GLBINDVERTEXARRAYPROC glBindVertexArray;
GLCLEARPROC           glClear;
GLCLEARCOLORPROC      glClearColor;
GLDRAWARRAYSPROC      glDrawArrays;

#define VERT_SHADER                          \
    "#version 330 core\n"                    \
    "void main() {\n"                        \
    "vec4 vertices[4] = vec4[4](\n"          \
    "    vec4(-1, -1, 0, 1),\n"              \
    "    vec4( 1, -1, 0, 1),\n"              \
    "    vec4(-1,  1, 0, 1),\n"              \
    "    vec4( 1,  1, 0, 1)\n"               \
    ");\n"                                   \
    "gl_Position = vertices[gl_VertexID];\n" \
    "}"

#define FRAG_SHADER        \
    "#version 330 core\n"  \
    "out vec4 color;\n"    \
    "void main() {\n"      \
    "color = vec4(1.0);\n" \
    "}"

static SDL_Window   *window     = NULL;
static SDL_Surface  *surface    = NULL;
static SDL_GLContext gl_context = NULL;

void SetWindowShapes(SDL_Rect rects[], int count)
{
    SDL_FillSurfaceRect(surface, NULL, 0x0);

    for (int i = 0; i < count; ++i)
    {
        SDL_FillSurfaceRect(surface, &rects[i], 0x000000ff);
    }

    SDL_SetWindowShape(window, surface);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("SDL_SetWindowShape test", 500, 500, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_OPENGL);

    if (!window)
    {
        return SDL_APP_FAILURE;
    }

    surface = SDL_CreateSurface(500, 500, SDL_PIXELFORMAT_ARGB32);

    if (!surface)
    {
        return SDL_APP_FAILURE;
    }

    SetWindowShapes((SDL_Rect[]){{0, 0, 250, 250}, {250, 250, 250, 250}}, 2);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    gl_context = SDL_GL_CreateContext(window);

    if (!gl_context)
    {
        return SDL_APP_FAILURE;
    }

    SDL_GL_MakeCurrent(window, gl_context);

    glCreateProgram   = (GLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
    glCreateShader    = (GLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
    glShaderSource    = (GLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
    glCompileShader   = (GLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
    glAttachShader    = (GLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
    glLinkProgram     = (GLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
    glDeleteShader    = (GLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
    glUseProgram      = (GLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
    glGenVertexArrays = (GLGENVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glGenVertexArrays");
    glBindVertexArray = (GLBINDVERTEXARRAYPROC)SDL_GL_GetProcAddress("glBindVertexArray");
    glClear           = (GLCLEARPROC)SDL_GL_GetProcAddress("glClear");
    glClearColor      = (GLCLEARCOLORPROC)SDL_GL_GetProcAddress("glClearColor");
    glDrawArrays      = (GLDRAWARRAYSPROC)SDL_GL_GetProcAddress("glDrawArrays");

    const char *vert_src = VERT_SHADER;
    const char *frag_src = FRAG_SHADER;

    int vao;
    int program     = glCreateProgram();
    int vert_shader = glCreateShader(0x8B31);
    int frag_shader = glCreateShader(0x8B30);

    glShaderSource(vert_shader, 1, &vert_src, NULL);
    glShaderSource(frag_shader, 1, &frag_src, NULL);
    glCompileShader(vert_shader);
    glCompileShader(frag_shader);
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    glUseProgram(program);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    glClear(0x4000);
    glClearColor(0, 0, 0, 1);
    glDrawArrays(0x0005, 0, 4);

    SDL_GL_SwapWindow(window);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate)
{
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
}
