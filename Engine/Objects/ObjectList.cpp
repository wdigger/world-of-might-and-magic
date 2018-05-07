#include "Engine/Objects/ObjectList.h"

#include "Engine/Graphics/Sprites.h"

struct ObjectList *pObjectList;

unsigned int ObjectList::ObjectIDByItemID(unsigned int uItemID) {
    unsigned int res = 0;

    for (ObjectDesc &object : vObjects) {
        if (uItemID == object.uObjectID) {
            break;
        }
        res++;
    }

    return res;
}

void ObjectList::InitializeSprites() {
    for (ObjectDesc &object : vObjects) {
        pSpriteFrameTable->InitializeSprite(object.uSpriteID);
    }
}

void ObjectList::InitializeColors() {
    for (ObjectDesc &object : vObjects) {
        object.uParticleTrailColor =
            ((unsigned int)object.uParticleTrailColorB) |
            ((unsigned int)object.uParticleTrailColorG << 8) |
            ((unsigned int)object.uParticleTrailColorR << 16);
    }
}

#pragma pack(push, 1)
struct ObjectDesc_mm6 {
    inline bool NoSprite() const { return uFlags & OBJECT_DESC_NO_SPRITE; }

    char field_0[32];
    int16_t uObjectID;
    int16_t uRadius;
    int16_t uHeight;
    int16_t uFlags;
    uint16_t uSpriteID;
    int16_t uLifetime;
    uint16_t uParticleTrailColor;
    int16_t uSpeed;
    char uParticleTrailColorR;
    char uParticleTrailColorG;
    char uParticleTrailColorB;
    char field_35_clr;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ObjectDesc_mm7 {
    inline bool NoSprite() const { return uFlags & OBJECT_DESC_NO_SPRITE; }

    char field_0[32];
    int16_t uObjectID;
    int16_t uRadius;
    int16_t uHeight;
    int16_t uFlags;
    uint16_t uSpriteID;
    int16_t uLifetime;
    uint32_t uParticleTrailColor;
    int16_t uSpeed;
    uint8_t uParticleTrailColorR;
    uint8_t uParticleTrailColorG;
    uint8_t uParticleTrailColorB;
    char field_35_clr;
    char field_36_clr;
    char field_37_clr;
};
#pragma pack(pop)

ObjectDesc::ObjectDesc(struct ObjectDesc_mm6 *src) {
    sName = src->field_0;
    uObjectID = src->uObjectID;
    uRadius = src->uRadius;
    uHeight = src->uHeight;
    uFlags = src->uFlags;
    uSpriteID = src->uSpriteID;
    uLifetime = src->uLifetime;
    uParticleTrailColor = src->uParticleTrailColor;
    uSpeed = src->uSpeed;
    uParticleTrailColorR = src->uParticleTrailColorR;
    uParticleTrailColorG = src->uParticleTrailColorG;
    uParticleTrailColorB = src->uParticleTrailColorB;
    field_35_clr = src->field_35_clr;
    field_36_clr = 0;
    field_37_clr = 0;
}

ObjectDesc::ObjectDesc(struct ObjectDesc_mm7 *src) {
    sName = src->field_0;
    uObjectID = src->uObjectID;
    uRadius = src->uRadius;
    uHeight = src->uHeight;
    uFlags = src->uFlags;
    uSpriteID = src->uSpriteID;
    uLifetime = src->uLifetime;
    uParticleTrailColor = src->uParticleTrailColor;
    uSpeed = src->uSpeed;
    uParticleTrailColorR = src->uParticleTrailColorR;
    uParticleTrailColorG = src->uParticleTrailColorG;
    uParticleTrailColorB = src->uParticleTrailColorB;
    field_35_clr = src->field_35_clr;
    field_36_clr = src->field_36_clr;
    field_37_clr = src->field_37_clr;
}

void ObjectList::FromFile(void *data_mm6, void *data_mm7, void *data_mm8) {
    static_assert(sizeof(ObjectDesc_mm6) == 52, "Wrong type size");
    static_assert(sizeof(ObjectDesc_mm7) == 56, "Wrong type size");

    unsigned int num_mm6_objs = data_mm6 ? *(uint32_t*)data_mm6 : 0;
    unsigned int num_mm7_objs = data_mm7 ? *(uint32_t*)data_mm7 : 0;
    unsigned int num_mm8_objs = data_mm8 ? *(uint32_t*)data_mm8 : 0;

    unsigned int uNumObjects = num_mm6_objs + num_mm7_objs + num_mm8_objs;
    assert(uNumObjects != 0);
    assert(num_mm8_objs == 0);

    ObjectDesc_mm7 *pObjects_mm7 = (ObjectDesc_mm7*)((char*)data_mm7 + 4);
    for (unsigned int i = 0; i < num_mm7_objs; ++i) {
        ObjectDesc dst(pObjects_mm7 + i);
        vObjects.push_back(dst);
    }

    ObjectDesc_mm6 *pObjects_mm6 = (ObjectDesc_mm6*)((char*)data_mm6 + 4);
    for (unsigned int i = 0; i < num_mm6_objs; ++i) {
        ObjectDesc dst(pObjects_mm6 + i);
        vObjects.push_back(dst);
    }
}
