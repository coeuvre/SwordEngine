#include "sword/render.h"

#include <glad/glad.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "context.h"

// ----------------------------------------------------------------------------
// Graphics Properties
//
SDAPI float SDGetCanvasWidth(void) {
  return SDGetViewportWidth() * SDGetPixelToPoint();
}

SDAPI float SDGetCanvasHeight(void) {
  return SDGetViewportHeight() * SDGetPixelToPoint();
}

SDAPI int SDGetViewportWidth(void) { return CTX->viewportWidth; }

SDAPI int SDGetViewportHeight(void) { return CTX->viewportHeight; }

SDAPI float SDGetPointToPixel(void) { return CTX->pointToPixel; }

SDAPI float SDGetPixelToPoint(void) { return CTX->pixelToPoint; }

// ----------------------------------------------------------------------------
// Image
// ----------------------------------------------------------------------------

SDAPI SDImage *SDLoadImage(const char *path) {
  SDImage *image = malloc(sizeof(SDImage));

  image->data = stbi_load(path, &image->width, &image->height, NULL, 4);
  if (image->data == NULL) {
    printf("Failed to load image %s\n", path);
    free(image);
    return NULL;
  }
  image->stride = 4 * image->width;
  image->format = SD_IMAGE_FORMAT_RGBA8;

  return image;
}

SDAPI void SDDestroyImage(SDImage **ptr) {
  SDImage *image = *ptr;
  stbi_image_free(image->data);

  *ptr = NULL;
}

// ----------------------------------------------------------------------------
// Texture
// ----------------------------------------------------------------------------

struct SDTexture {
  GLuint id;
  int actualWidth;
  int actualHeight;
  int width;
  int height;
};

static void UploadImageToGPU(SDTexture *texture, const void *data, int width,
                             int height, int stride, int format) {
  glGenTextures(1, &texture->id);
  glBindTexture(GL_TEXTURE_2D, texture->id);

  texture->actualWidth = (int)SDNextPow2F((float)width);
  texture->actualHeight = height;

  int texStride = 0;
  GLint numberOfPixels = 0;
  GLint internalFormat = 0;
  GLenum glFormat = 0;

  switch (format) {
    case SD_IMAGE_FORMAT_RGBA8: {
      texStride = texture->actualWidth * 4;
      numberOfPixels = texture->actualWidth;

      internalFormat = GL_SRGB8_ALPHA8;
      glFormat = GL_RGBA;
    } break;
    case SD_IMAGE_FORMAT_A8: {
      texStride = (int)(SDCeilF(texture->actualWidth / 4.0f) * 4.0f);
      numberOfPixels = texStride;

      internalFormat = GL_R8;
      glFormat = GL_RED;

      GLint swizzleMask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
      glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    } break;
  }

  SDAssert(stride <= texStride);
  SDAssert(tex->actualWidth % 2 == 0);
  SDAssert(texStride % 4 == 0);

  size_t texBufLen = (size_t)texStride * texture->actualHeight;
  unsigned char *texBuf = malloc(texBufLen);
  memset(texBuf, 0, texBufLen);

  unsigned char *dstRow = texBuf;
  const unsigned char *srcRow = ((unsigned char *)data) + stride * (height - 1);

  // Flip image vertically
  for (int y = 0; y < height; ++y) {
    memcpy(dstRow, srcRow, (size_t)stride);
    dstRow += texStride;
    srcRow -= stride;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, numberOfPixels);
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture->actualWidth,
               texture->actualHeight, 0, glFormat, GL_UNSIGNED_BYTE, texBuf);

  free(texBuf);
}

static SDTexture *LoadTextureFromMemory(const void *data, int width, int height,
                                        int stride, int format) {
  SDTexture *texture = malloc(sizeof(SDTexture));
  texture->width = width;
  texture->height = height;

  UploadImageToGPU(texture, data, width, height, stride, format);

  return texture;
}

SDAPI SDTexture *SDLoadTexture(const char *path) {
  SDImage *image = SDLoadImage(path);

  if (image == NULL) {
    return NULL;
  }

  SDTexture *texture = LoadTextureFromMemory(
      image->data, image->width, image->height, image->stride, image->format);

  SDDestroyImage(&image);

  return texture;
}

SDAPI void SDDestroyTexture(SDTexture **ptr) {
  SDTexture *texture = *ptr;

  glDeleteTextures(1, &texture->id);

  free(texture);

  *ptr = NULL;
}