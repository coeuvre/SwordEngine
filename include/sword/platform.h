#ifndef SD_PLATFORM_H
#define SD_PLATFORM_H

#include "sword/def.h"
#include "sword/entity.h"

typedef void (*SDLoadCallback)(void *gameState);
typedef void (*SDUpdateCallback)(void *gameState);
typedef void (*SDRenderCallback)(void *gameState);

SDAPI void SDSetExitOnEsc(int exitOnEsc);
SDAPI void SDSetGameState(void *gameState);
SDAPI void SDSetLoadCallback(SDLoadCallback load);
SDAPI void SDSetUpdateCallback(SDUpdateCallback update);
SDAPI void SDSetRenderCallback(SDRenderCallback render);

SDAPI void SDRun(void);

#endif  // SD_PLATFORM_H
