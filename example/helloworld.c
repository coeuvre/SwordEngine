#include "stdio.h"

#include "sword/sword.h"

static SDEntityRef createRootEntity(void) {
  SDEntityRef root = SDCreateEntity("Root");

  SDTransformComponent *transform = SDAddComponent(root, Transform);
  SDSpriteComponent *sprite = SDAddComponent(root, Sprite);

  return root;
}

int main(int argc, char *argv[]) {
  SDConfig config = SDDefaultConfig();
  SDInit(&config);

  printf("Viewport size: %dx%d (px)\n", SDGetViewportWidth(),
         SDGetViewportHeight());
  printf("DPI Scale factor: %.1f\n", SDGetPointToPixel());

  SDRunScene(createRootEntity());

  SDQuit();

  return 0;
}
