#include "stdio.h"

#include "sword/sword.h"

int main(int argc, char *argv[]) {
  SDConfig config = SDDefaultConfig();
  SDInit(&config);

  printf("Viewport size: %dx%d (px)\n", SDGetViewportWidth(),
         SDGetViewportHeight());
  printf("DPI Scale factor: %.1f\n", SDGetPointToPixel());

  SDRunScene();

  SDQuit();

  return 0;
}
