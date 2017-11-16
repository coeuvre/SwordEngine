#ifndef SD_ASSETS_H
#define SD_ASSETS_H

#include "sword/def.h"

typedef enum SDAssetType {
  SD_ASSET_TEXTURE,
} SDAssetType;

typedef enum SDAssetRefType {
  SD_ASSET_REF_EMPTY,
  SD_ASSET_REF_PATH,
} SDAssetRefType;

typedef struct SDAssertRef {
  SDAssetType assetType;
  SDAssetRefType type;
  union {
    const char *path;
  };
} SDAssetRef;

SDINLINE SDAssetRef SDEmptyTextureRef(void) {
  return (SDAssetRef){
      .assetType = SD_ASSET_TEXTURE,
      .type = SD_ASSET_REF_EMPTY,
      .path = 0,
  };
}

SDINLINE SDAssetRef SDMakeTextureRefFromPath(const char *path) {
  return (SDAssetRef){
      .assetType = SD_ASSET_TEXTURE,
      .type = SD_ASSET_REF_PATH,
      .path = path,
  };
}

#endif  // SD_ASSETS_H
