#ifndef SD_RENDER_H
#define SD_RENDER_H

#include "def.h"
#include "math.h"

typedef struct Color {
  float r, g, b, a;
} Color;

// ----------------------------------------------------------------------------
// Graphics Properties
// ----------------------------------------------------------------------------

// Get device size in pixel
SDAPI int SDGetDeviceWidth(void);
SDAPI int SDGetDeviceHeight(void);

// Get drawing size in point
SDAPI int SDGetCanvasWidth(void);
SDAPI int SDGetCanvasHeight(void);

// Get viewport size in pixel
SDAPI int SDGetViewportWidth(void);
SDAPI int SDGetViewportHeight(void);

// ----------------------------------------------------------------------------
// Render State
// ----------------------------------------------------------------------------

SDAPI void SDPushState(void);
SDAPI void SDPopState(void);

SDAPI void SDPushMatrix(Mat3 mat);
SDAPI void SDPopMatrix(void);

SDAPI void SDSetFillColor(Color fill);
SDAPI void SDSetStrokeColor(Color stroke);
SDAPI void SDSetTintColor(Color tint);

SDAPI void SDSetTextureAnchor(float x, float y);

// ----------------------------------------------------------------------------
// Image
// ----------------------------------------------------------------------------

// Image stored in CPU memory
typedef struct Image {
  int width;
  int height;
  int format;
  void *data;
} Image;

SDAPI Image *LoadImage(const char *path);
SDAPI Image *DestroyImage(Image **image);

// ----------------------------------------------------------------------------
// Texture
// ----------------------------------------------------------------------------

// Image stored in GPU memory
typedef struct Texture Texture;

// Load texture from disk at given path
SDAPI Texture *SDLoadTexture(const char *path);
// Load Texture from Image
SDAPI Texture *SDLoadTextureFromImage(const Image *image);
SDAPI void SDDestoryTexture(Texture **texture);
SDAPI void SDDrawTexture(const Texture *texture, float x, float y);
SDAPI void SDDrawTextureSub(const Texture *texture, Rect rect, float x,
                            float y);

// ----------------------------------------------------------------------------
// Shape
// ----------------------------------------------------------------------------

#endif  // SD_RENDER_H
