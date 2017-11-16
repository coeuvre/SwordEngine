#include "sword/platform.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "context.h"
#include "sword/entity.h"

SDAPI SDConfig SDDefaultConfig(void) {
  SDConfig config = {
      .window = {.width = 1280,
                 .height = 720,
                 .title = "Sword",
                 .supportHiDPI = 1},
      .exitOnEsc = 0,
  };

#ifdef SD_DEBUG
  config.exitOnEsc = 1;
#endif

  return config;
}

static void InitWindow(const SDConfig *config) {
  CTX->viewportWidth = config->window.width;
  CTX->viewportHeight = config->window.height;

#ifdef SD_PLATFORM_WIN32
  if (config->window.supportHiDPI) {
    SetProcessDPIAware();

    HDC screen = GetDC(0);
    int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
    ReleaseDC(0, screen);

    CTX->pointToPixel = dpiX / 96.0f;
    CTX->viewportWidth = (int)(CTX->viewportWidth * CTX->pointToPixel);
    CTX->viewportHeight = (int)(CTX->viewportHeight * CTX->pointToPixel);
  }
#endif

  SDL_Init(SDL_INIT_VIDEO);

  // Use this function to set an OpenGL window attribute before window creation.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  int flags = SDL_WINDOW_OPENGL;
  if (config->window.supportHiDPI) {
    flags |= SDL_WINDOW_ALLOW_HIGHDPI;
  }
  CTX->window = SDL_CreateWindow(config->window.title, SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, CTX->viewportWidth,
                                 CTX->viewportHeight, flags);

  if (!CTX->window) {
    printf("Failed to create SDL window: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  CTX->glContext = SDL_GL_CreateContext(CTX->window);

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
  SDL_GL_GetDrawableSize(CTX->window, &drawableWidth, &drawableHeight);
  CTX->pointToPixel = drawableWidth * 1.0f / CTX->viewportWidth;
#endif

  CTX->pixelToPoint = 1.0f / CTX->pointToPixel;
}

SDAPI void SDInit(const SDConfig *config) {
  CTX = malloc(sizeof(SDContext));
  memset(CTX, 0, sizeof(*CTX));

  InitWindow(config);

  CTX->rc = CreateRenderContext(CTX->viewportWidth, CTX->viewportHeight,
                                CTX->pixelToPoint);
  CTX->testTexture = SDLoadTexture("assets/background_day.png");
}

SDAPI void SDQuit(void) {
  SDDestroyTexture(&CTX->testTexture);

  free(CTX);
  CTX = NULL;
}

static void ProcessSystemEvent(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
        CTX->isRunning = 0;
      } break;

      case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          CTX->isRunning = 0;
        }
      } break;

      default:
        break;
    }
  }
}

static void Render(void) {
  SDDrawTextureParams params = SDMakeDrawTextureParams(CTX->testTexture);
  SDDrawTexture(&params);
}

SDAPI void SDRunScene(SDEntityRef entityRef) {
  CTX->isRunning = 1;

  while (CTX->isRunning) {
    ProcessSystemEvent();

    Render();

    SDL_GL_SwapWindow(CTX->window);
  }
}
