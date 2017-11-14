#ifndef SD_PLATFORM_H
#define SD_PLATFORM_H

#include "sword/def.h"

typedef struct SDWindowConfig {
  int width;          // window width in point
  int height;         // window height in point
  const char *title;  // window title
  int supportHiDPI;   // support hidpi mode
} SDWindowConfig;

typedef struct SDConfig {
  SDWindowConfig window;
  int exitOnEsc;  // Exit game when Esc pressed?
} SDConfig;

// Get default configuration
SDAPI SDConfig SDDefaultConfig(void);

SDAPI void SDInit(const SDConfig *config);
SDAPI void SDQuit(void);

SDAPI void SDRunScene(void);

#endif  // SD_PLATFORM_H
