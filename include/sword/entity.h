#ifndef SD_ENTITY_H
#define SD_ENTITY_H

#include "sword/assets.h"
#include "sword/def.h"
#include "sword/math.h"

typedef struct SDEntity SDEntity;
typedef SDEntity *SDEntityRef;

typedef enum SDComponentType {
  SD_COMPONENT_Transform = 0,
  SD_COMPONENT_Sprite,
  SD_COMPONENT_COUNT,
} SDComponentType;

typedef struct SDTransfromComponent {
  SDFloat x;
  SDFloat y;
} SDTransformComponent;

SDINLINE SDTransformComponent SDDefaultTransformComponent(void) {
  return (SDTransformComponent){
      .x = 0.0f,
      .y = 0.0f,
  };
}

typedef struct SDSpriteComponent {
  SDAssetRef textureRef;
  SDRect region;  // Source rect in texture space
  SDVec2 anchor;  // Anchor point in texture space
} SDSpriteComponent;

SDINLINE SDSpriteComponent SDDefaultSpriteComponent(void) {
  return (SDSpriteComponent){
      .textureRef = SDEmptyTextureRef(),
      .region = SDZeroRect(),
      .anchor = SDZeroVec2(),
  };
}

SDAPI SDEntityRef SDCreateEntity(const char *name);
SDAPI void SDDestroyEntity(SDEntityRef *entityRef);

#define SDAddComponent(entityRef, componentType)      \
  ((SD##componentType##Component *)SDAddComponentRaw( \
      (entityRef), SD_COMPONENT_##componentType))

#define SDGetComponent(entityRef, componentType)      \
  ((SD##componentType##Component *)SDGetComponentRaw( \
      (entityRef), SD_COMPONENT_##componentType))

SDAPI void *SDAddComponentRaw(SDEntityRef entityRef,
                              SDComponentType componentType);

SDAPI void *SDGetComponentRaw(SDEntityRef entityRef,
                              SDComponentType componentType);

#endif  // SD_ENTITY_H
