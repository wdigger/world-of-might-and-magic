#pragma once

#include <cstdint>
#include <vector>

#include "Engine/Strings.h"

/*  321 */
enum DECORATION_DESC_FLAGS {
    DECORATION_DESC_MOVE_THROUGH = 0x0001,
    DECORATION_DESC_DONT_DRAW = 0x0002,
    DECORATION_DESC_FLICKER_SLOW = 0x0004,
    DECORATION_DESC_FICKER_AVERAGE = 0x0008,
    DECORATION_DESC_FICKER_FAST = 0x0010,
    DECORATION_DESC_MARKER = 0x0020,
    DECORATION_DESC_SLOW_LOOP = 0x0040,
    DECORATION_DESC_EMITS_FIRE = 0x0080,
    DECORATION_DESC_SOUND_ON_DAWN = 0x0100,
    DECORATION_DESC_SOUND_ON_DUSK = 0x0200,
    DECORATION_DESC_EMITS_SMOKE = 0x0400,
};

/*   54 */
#pragma pack(push, 1)
struct DecorationDesc_mm6 {
    inline bool CanMoveThrough() {
        return (uFlags & DECORATION_DESC_MOVE_THROUGH) != 0;
    }
    inline bool DontDraw() { return (uFlags & DECORATION_DESC_DONT_DRAW) != 0; }
    inline bool SoundOnDawn() {
        return (uFlags & DECORATION_DESC_SOUND_ON_DAWN) != 0;
    }
    inline bool SoundOnDusk() {
        return (uFlags & DECORATION_DESC_SOUND_ON_DUSK) != 0;
    }

    char pName[32];
    char pDescription[32];
    int16_t uType;
    uint16_t uDecorationHeight;
    int16_t uRadius;
    int16_t uLightRadius;
    uint16_t uSpriteID;
    int16_t uFlags;
    int16_t uSoundID;
    int16_t _pad;
};

struct DecorationDesc : public DecorationDesc_mm6 {
    uint8_t uColoredLightRed;
    uint8_t uColoredLightGreen;
    uint8_t uColoredLightBlue;
    char __padding;
};
#pragma pack(pop)

class DecorationList {
 public:
    inline DecorationList() {}

    void ToFile();
    void FromFile(void *data_mm6, void *data_mm7, void *data_mm8);
    void InitializeDecorationSprite(struct DecorationDesc *pDecorationDesc);
    uint16_t GetDecorIdByName(const char *pName);
    DecorationDesc *GetDecoration(unsigned int index) {
        return &pDecorations[index];
    }

 protected:
    std::vector<DecorationDesc> pDecorations;
};

extern class DecorationList *pDecorationList;

void RespawnGlobalDecorations();
