#ifndef SD_RENDER_H
#define SD_RENDER_H

#include "sword/def.h"
#include "sword/math.h"

// Normalized color
typedef struct SDColor {
  SDFloat r, g, b, a;
} SDColor;

SDINLINE SDColor SDRGBA(SDFloat r, SDFloat g, SDFloat b, SDFloat a) {
  return (SDColor){r, g, b, a};
}

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

enum {
  SD_IMAGE_FORMAT_RGBA8,
  SD_IMAGE_FORMAT_A8,
};

// Image stored in CPU memory
typedef struct SDImage {
  int width;
  int height;
  int stride;
  int format;
  void *data;
} SDImage;

SDAPI SDImage *SDLoadImage(const char *path);
SDAPI void SDDestroyImage(SDImage **image);

// ----------------------------------------------------------------------------
// Texture
// ----------------------------------------------------------------------------

// Image stored in GPU memory
typedef struct SDTexture SDTexture;

// Load texture from disk at given path
SDAPI SDTexture *SDLoadTexture(const char *path);
// Load Texture from Image
SDAPI SDTexture *SDLoadTextureFromImage(const SDImage *image);
SDAPI void SDDestroyTexture(SDTexture **texture);

typedef struct SDDrawTextureParams {
  SDTexture *texture;
  SDMat3 transform;
  SDRect dstRect;  // Destination rect in world space
  SDRect srcRect;  // Source rect in texture space (pixel)
  SDColor tintColor;
} SDDrawTextureParams;

SDAPI SDDrawTextureParams SDMakeDrawTextureParams(SDTexture *texture);

/**
 * Code Example:
 *
 * SDTexture *texture = SDLoadTexture("xxx.png");
 * SDDrawTextureParams dtp = SDMakeDrawTextureParams(texture);
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

SDAPI SDDrawRectParams SDMakeDrawRectParams(SDRect rect);

SDAPI void SDDrawRect(const SDDrawRectParams *params);

#endif  // SD_RENDER_H
