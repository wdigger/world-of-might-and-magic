#pragma once

#include <cstdint>
#include <vector>

#include "Engine/Strings.h"
#include "Engine/VectorTypes.h"

enum LEVEL_DECORATION_FLAGS : uint16_t {
    LEVEL_DECORATION_TRIGGERED_BY_TOUCH = 0x01,
    LEVEL_DECORATION_TRIGGERED_BY_MONSTER = 0x02,
    LEVEL_DECORATION_TRIGGERED_BY_OBJECT = 0x04,
    LEVEL_DECORATION_VISIBLE_ON_MAP = 0x08,
    LEVEL_DECORATION_CHEST = 0x10,
    LEVEL_DECORATION_INVISIBLE = 0x20,
    LEVEL_DECORATION_OBELISK_CHEST = 0x40,
};

struct LevelDecoration {
    LevelDecoration(struct LevelDecoration_mm7 *pDecoration, const String &pDescName);
    int GetGlobalEvent();
    bool IsInteractive();
    bool IsObeliskChestActive();

    uint16_t uDecorationDescID;
    uint16_t uFlags;
    Vec3_int_ vPosition;
    int32_t field_10_y_rot;
    uint16_t uCog;
    uint16_t uEventID;
    uint16_t uTriggerRange;
    int16_t field_1A;
    int16_t _idx_in_stru123;
    int16_t field_1E;
};

extern std::vector<LevelDecoration> pLevelDecorations;
extern LevelDecoration* activeLevelDecoration;

char *LevelDecorationsSerialize(char *pData);
char *LevelDecorationsDeserialize(char *pData);

void SetDecorationSprite(uint16_t uCog, bool bHide, const char *pFileName);
