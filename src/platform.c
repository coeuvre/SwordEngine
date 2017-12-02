#include "sword/platform.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "context.h"
#include "sword/entity.h"

typedef struct WindowConfig {
  int width;          // window width in point
  int height;         // window height in point
  const char *title;  // window title
  int supportHiDPI;   // support hidpi mode
} WindowConfig;

typedef struct Config {
  WindowConfig window;
  int exitOnEsc;  // Exit game when Esc pressed?
  SDLoadCallback load;
  SDUpdateCallback update;
  SDUpdateCallback render;
  void *gameState;
} Config;

Config CONFIG = {
    .window = {.width = 1280,
               .height = 720,
               .title = "Sword",
               .supportHiDPI = 1},
    .exitOnEsc = 0,
    .update = 0,
    .load = 0,
    .render = 0,
    .gameState = 0,
};

static void InitWindow(const WindowConfig *window) {
  CTX.viewportWidth = window->width;
  CTX.viewportHeight = window->height;

#ifdef SD_PLATFORM_WIN32
  if (window->supportHiDPI) {
    SetProcessDPIAware();

    HDC screen = GetDC(0);
    int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
    ReleaseDC(0, screen);

    CTX.pointToPixel = dpiX / 96.0f;
    CTX.viewportWidth = (int)(CTX.viewportWidth * CTX.pointToPixel);
    CTX.viewportHeight = (int)(CTX.viewportHeight * CTX.pointToPixel);
  }
#endif

  SDL_Init(SDL_INIT_VIDEO);

  // Use this function to set an OpenGL window attribute before window creation.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  int flags = SDL_WINDOW_OPENGL;
  if (window->supportHiDPI) {
    flags |= SDL_WINDOW_ALLOW_HIGHDPI;
  }
  CTX.window = SDL_CreateWindow(window->title, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, CTX.viewportWidth,
                                CTX.viewportHeight, flags);

  if (!CTX.window) {
    printf("Failed to create SDL window: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  CTX.glContext = SDL_GL_CreateContext(CTX.window);

  SDL_GL_SetSwapInterval(1);

  if (gladLoadGLLoader(&SDL_GL_GetProcAddress) == 0) {
    printf("Failed to load OpenGL\n");
    exit(EXIT_FAILURE);
  }

#ifdef GLAD_DEBUG
  printf("Glad Debug Mode\n");
#endif

  if (GLVersion.major < 3) {
    printf("Your system doesn't support OpenGL >= 3!\n");
    exit(EXIT_FAILURE);
  }

  printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
         glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifndef SD_PLATFORM_WIN32
  int drawableWidth, drawableHeight;
  SDL_GL_GetDrawableSize(CTX.window, &drawableWidth, &drawableHeight);
  CTX.pointToPixel = drawableWidth * 1.0f / CTX.viewportWidth;
#endif

  CTX.pixelToPoint = 1.0f / CTX.pointToPixel;
}

static void ProcessSystemEvent(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
        CTX.isRunning = 0;
      } break;

      case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_ESCAPE && CONFIG.exitOnEsc) {
          CTX.isRunning = 0;
        }
      } break;

      default:
        break;
    }
  }
}

static void Update(void) {
  if (CONFIG.update) {
    CONFIG.update(CONFIG.gameState);
  }
}

static void Render(void) {
  if (CONFIG.render) {
    CONFIG.render(CONFIG.gameState);
  }
}

SDAPI void SDSetExitOnEsc(int exitOnEsc) { CONFIG.exitOnEsc = exitOnEsc; }

SDAPI void SDSetGameState(void *gameState) { CONFIG.gameState = gameState; }

SDAPI void SDSetLoadCallback(SDLoadCallback load) { CONFIG.load = load; }

SDAPI void SDSetUpdateCallback(SDUpdateCallback update) {
  CONFIG.update = update;
}

SDAPI void SDSetRenderCallback(SDRenderCallback render) {
  CONFIG.render = render;
}

SDAPI void SDRun(void) {
  InitWindow(&CONFIG.window);

  CTX.rc = CreateRenderContext(CTX.viewportWidth, CTX.viewportHeight,
                               CTX.pixelToPoint);

  if (CONFIG.load) {
    CONFIG.load(CONFIG.gameState);
  }

  CTX.isRunning = 1;
  while (CTX.isRunning) {
    ProcessSystemEvent();

    Update();
    Render();

    SDL_GL_SwapWindow(CTX.window);
  }
}