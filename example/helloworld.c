#include "stdio.h"

#include "sword/sword.h"

typedef struct GameState {
  int counter;
  SDTexture *texture;
} GameState;

static void Load(GameState *gameState) {
  printf("Viewport size: %dx%d px\n", SDGetViewportWidth(),
         SDGetViewportHeight());
  printf("Canvas size: %.0fx%.0f pt\n", SDGetCanvasWidth(),
         SDGetCanvasHeight());
  printf("HiDPI scale: %.0f\n", SDGetPointToPixel());

  gameState->texture = SDLoadTexture("assets/background_day.png");
}

static void Update(GameState *gameState) {
  gameState->counter++;

  printf("%d\n", gameState->counter);
}

static void Render(GameState *gameState) {
  SDDrawTextureParams params = SDMakeDrawTextureParams(gameState->texture);
  SDDrawTexture(&params);
}

int main(int argc, char *argv[]) {
  GameState gameState = {0};

  SDSetExitOnEsc(1);
  SDSetGameState(&gameState);
  SDSetLoadCallback((SDLoadCallback)Load);
  SDSetUpdateCallback((SDUpdateCallback)Update);
  SDSetRenderCallback((SDRenderCallback)Render);

  SDRun();

  return 0;
}
