#include "sword/render.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "context.h"

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