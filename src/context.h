#ifndef SD_CONTEXT_H
#define SD_CONTEXT_H

#include <stdio.h>
#include <stdlib.h>

#ifdef SD_PLATFORM_WIN32
#include <windows.h>
#endif

#include "sword/render.h"

struct SDL_Window;
struct SDL_GLContext;

typedef struct RenderContext RenderContext;

typedef struct Context {
  int isRunning;

  float pointToPixel;
  float pixelToPoint;

  int viewportWidth;
  int viewportHeight;

  struct SDL_Window *window;
  struct SDL_GLContext *glContext;

  RenderContext *rc;
} Context;

extern Context CTX;

extern RenderContext *CreateRenderContext(int viewportWidth, int viewportHeight,
                                          float pixelToPoint);
#endif  // SD_CONTEXT_H
