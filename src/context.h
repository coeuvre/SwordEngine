#ifndef SD_CONTEXT_H
#define SD_CONTEXT_H

#include <stdio.h>
#include <stdlib.h>

#ifdef SD_PLATFORM_WIN32
#include <windows.h>
#endif

#include <SDL2/SDL.h>
#include <glad/glad.h>

typedef struct SDContext {
  float pointToPixel;

  int viewportWidth;
  int viewportHeight;

  SDL_Window *window;
  SDL_GLContext *glContext;
} SDContext;

extern SDContext *CTX;

#endif  // SD_CONTEXT_H
