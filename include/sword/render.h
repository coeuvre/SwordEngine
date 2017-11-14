#ifndef SD_RENDER_H
#define SD_RENDER_H

#include "def.h"
#include "math.h"

// Normalized color
typedef struct SDColor {
  SDFloat r, g, b, a;
} SDColor;

// ----------------------------------------------------------------------------
// Graphics Properties
// ----------------------------------------------------------------------------

// Get device size in pixel
SDAPI int SDGetDeviceWidth(void);
SDAPI int SDGetDeviceHeight(void);

// Get viewport size in pixel
SDAPI int SDGetViewportWidth(void);
SDAPI int SDGetViewportHeight(void);

// Get drawing size in point
SDAPI float SDGetCanvasWidth(void);
SDAPI float SDGetCanvasHeight(void);

SDAPI float SDGetPointToPixel(void);
SDAPI float SDGetPixelToPoint(void);

// ----------------------------------------------------------------------------
// Render State
// ----------------------------------------------------------------------------

SDAPI void SDPushState(void);
SDAPI void SDPopState(void);

SDAPI void SDPushMatrix(SDMat3 mat);
SDAPI void SDPopMatrix(void);

// ----------------------------------------------------------------------------
// Image
// ----------------------------------------------------------------------------

// Image stored in CPU memory
typedef struct SDImage {
  int width;
  int height;
  int format;
  void *data;
} SDImage;

SDAPI SDImage *SDLoadImage(const char *path);
SDAPI SDImage *SDDestroyImage(SDImage **image);

// ----------------------------------------------------------------------------
// Texture
// ----------------------------------------------------------------------------

// Image stored in GPU memory
typedef struct SDTexture SDTexture;

// Load texture from disk at given path
SDAPI SDTexture *SDLoadTexture(const char *path);
// Load Texture from Image
SDAPI SDTexture *SDLoadTextureFromImage(const SDImage *image);
SDAPI void SDDestoryTexture(SDTexture **texture);

typedef struct SDDrawTextureParams {
  SDTexture *texture;
  SDRect dstRect;  // Destination rect in world space
  SDRect srcRect;  // Source rect in texture space (pixel)
  SDColor tintColor;
} SDDrawTextureParams;

SDAPI SDDrawTextureParams SDDefaultDrawTextureParams(SDTexture *texture);

/**
 * Code Example:
 *
 * SDTexture *texture = SDLoadTexture("xxx.png");
 * SDDrawTextureParams dtp = SDDefaultDrawTextureParams(texture);
 * SDDrawTexture(&dtp);
 * SDDestroyTexture(&texture);
 */
SDAPI void SDDrawTexture(const SDDrawTextureParams *params);

// ----------------------------------------------------------------------------
// Shape
// ----------------------------------------------------------------------------
typedef struct SDDrawRectParams {
  SDRect rect;
  SDFloat borderWidth;
  SDFloat cornerRadius;
  SDColor strokeColor;
  SDColor fillColor;
} SDDrawRectParams;

SDAPI SDDrawRectParams SDDefaultDrawRectParams(SDRect rect);

SDAPI void SDDrawRect(const SDDrawRectParams *params);

#endif  // SD_RENDER_H
