#include "render.h"

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

SDAPI float SDGetPixelToPoint(void) { return 1.0f / CTX->pointToPixel; }
