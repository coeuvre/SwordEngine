#include "platform.h"

#include "context.h"
#include "render.h"

SDAPI SDConfig SDDefaultConfig(void) {
  SDConfig config = {
      .window = {.width = 1280,
                 .height = 720,
                 .title = "Sword Engine",
                 .supportHiDPI = 1},
  };
  return config;
}

SDAPI void SDInit(const SDConfig *config) {
  CTX = malloc(sizeof(SDContext));
  memset(CTX, 0, sizeof(*CTX));

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
  SDL_GL_GetDrawableSize(CTX->sdlWindow, &drawableWidth, &drawableHeight);
  CTX->pointToPixel = drawableWidth * 1.0f / CTX->viewportWidth;
#endif
}

SDAPI void SDQuit(void) {
  free(CTX);
  CTX = NULL;
}
