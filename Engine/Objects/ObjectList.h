#pragma once

#include <cstdint>
#include <vector>

#include "Engine/Strings.h"

enum OBJECT_DESC_FLAGS {
    OBJECT_DESC_NO_SPRITE = 0x1,
    OBJECT_DESC_NO_COLLISION = 0x2,
    OBJECT_DESC_TEMPORARY = 0x4,
    OBJECT_DESC_SFT_LIFETIME = 0x8,
    OBJECT_DESC_UNPICKABLE = 0x10,
    OBJECT_DESC_NO_GRAVITY = 0x20,
    OBJECT_DESC_INTERACTABLE = 0x40,
    OBJECT_DESC_BOUNCE = 0x80,
    OBJECT_DESC_TRIAL_PARTICLE = 0x100,
    OBJECT_DESC_TRIAL_FIRE = 0x200,
    OBJECT_DESC_TRIAL_LINE = 0x400,
};

class ObjectDesc {
 public:
    ObjectDesc(struct ObjectDesc_mm6 *src);
    ObjectDesc(struct ObjectDesc_mm7 *src);
    bool NoSprite() const { return uFlags & OBJECT_DESC_NO_SPRITE; }

    String sName;
    unsigned int uObjectID;
    unsigned int uRadius;
    unsigned int uHeight;
    unsigned int uFlags;
    unsigned int uSpriteID;
    unsigned int uLifetime;
    uint32_t uParticleTrailColor;
    unsigned int uSpeed;
    uint8_t uParticleTrailColorR;
    uint8_t uParticleTrailColorG;
    uint8_t uParticleTrailColorB;
    char field_35_clr;
    char field_36_clr;
    char field_37_clr;
};

class ObjectList {
 public:
    inline ObjectList() {}

    void FromFile(void *data_mm6, void *data_mm7, void *data_mm8);
    void InitializeSprites();
    void InitializeColors();
    unsigned int ObjectIDByItemID(unsigned int uItemID);

 public:
    std::vector<ObjectDesc> vObjects;
};

extern ObjectList *pObjectList;
