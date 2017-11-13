#include "stdio.h"

#include "sword.h"

int main(int argc, char *argv[]) {
  SDConfig config = SDDefaultConfig();
  SDInit(&config);

  printf("Viewport size: %dx%d\n", SDGetViewportWidth(), SDGetViewportHeight());
  printf("DPI Scale factor: %f\n", SDGetPointToPixel());

  SDQuit();

  return 0;
}
