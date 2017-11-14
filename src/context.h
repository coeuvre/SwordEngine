#ifndef SD_CONTEXT_H
#define SD_CONTEXT_H

#include <stdio.h>
#include <stdlib.h>

#ifdef SD_PLATFORM_WIN32
#include <windows.h>
#endif

struct SDL_Window;
struct SDL_GLContext;

typedef struct SDContext {
  int isRunning;

  float pointToPixel;
  float pixelToPoint;

  int viewportWidth;
  int viewportHeight;

  struct SDL_Window *window;
  struct SDL_GLContext *glContext;
} SDContext;

extern SDContext *CTX;

#endif  // SD_CONTEXT_H
