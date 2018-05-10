#include "Engine/Graphics/Level/Decoration.h"

#include <cstdlib>

#include "Engine/Engine.h"
#include "Engine/Graphics/DecorationList.h"
#include "Engine/Party.h"

std::vector<LevelDecoration> pLevelDecorations;
LevelDecoration* activeLevelDecoration;

//----- (00450929) --------------------------------------------------------
int LevelDecoration::GetGlobalEvent() {
    // LevelDecoration *v1; // esi@1
    // signed int v2; // eax@1
    // int v3; // eax@5
    // int v4; // eax@6
    // int v5; // eax@7
    // int v6; // eax@8
    // int v7; // eax@9
    // int result; // eax@14
    // int v9; // eax@18
    // int v10; // eax@19
    // int v11; // eax@20
    // int v12; // eax@21
    // int v13; // eax@22

    switch (uDecorationDescID) {
        case 0:
        case 1:
        case 2:
        case 3:
            return 0;

        case 4:
            return 16;  // dec01 "Trash Pile"
        case 5:
            return 32;  // dec02 "Campfire"
        case 6:
            return 12 + rand() % 4;  // dec03 "Cauldron"
        case 7:
        case 8:
        case 9:
        case 10:
            return 0;

        case 11:
            return 34;  // dec08 "Fruit plate"
        case 12:
            return 0;

        case 13:
            return 17;  // dec10 "Trash Pile"
        case 14:
            return 18;  // dec11 "Filth"
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            return 0;

        case 24:
            return 36;  // dec21 "Keg"
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
            return 0;

        case 34:
            return 4 + rand() % 6;  // dec32 "Barrel"
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
        case 60:
        case 61:
        case 62:
        case 63:
        case 64:
        case 65:
        case 66:
        case 67:
        case 68:
        case 69:
        case 70:
        case 71:
        case 72:
        case 73:
        case 74:
        case 75:
        case 76:
        case 77:
        case 78:
        case 79:
        case 80:
        case 81:
        case 82:
        case 83:
        case 84:
        case 85:
        case 86:
        case 87:
        case 88:
        case 89:
        case 90:
        case 91:
        case 92:
        case 93:
        case 94:
        case 95:
        case 96:
        case 97:
        case 98:
        case 99:
        case 100:
        case 101:
        case 102:
        case 103:
        case 104:
        case 105:
        case 106:
        case 107:
        case 108:
        case 109:
        case 110:
        case 111:
        case 112:
        case 113:
        case 114:
        case 115:
        case 116:
        case 117:
        case 118:
        case 119:
        case 120:
        case 121:
        case 122:
        case 123:
        case 124:
        case 125:
        case 126:
        case 127:
        case 128:
        case 129:
        case 130:
        case 131:
        case 132:
        case 133:
        case 134:
        case 135:
        case 136:
        case 137:
        case 138:
        case 139:
        case 140:
        case 141:
        case 142:
        case 143:
        case 144:
        case 145:
        case 146:
        case 147:
        case 148:
        case 149:
        case 150:
        case 151:
        case 152:
        case 153:
        case 154:
        case 155:
        case 156:
        case 157:
        case 158:
        case 159:
        case 160:
        case 161:
        case 162:
        case 163:
        case 164:
        case 165:
        case 166:
        case 167:
        case 168:
        case 169:
        case 170:
        case 171:
        case 172:
        case 173:
        case 174:
        case 175:
        case 176:
        case 177:
        case 178:
        case 179:
        case 180:
        case 181:
        case 182:
        case 183:

        case 184:
            return 33;  // dec24 "Campfire"
        case 185:
        case 186:
            return 0;

        case 187:  // dec88 "Mushroom"
        case 190:  // dec91 "Mushroom"
            return 37;

        case 188:
        case 189:
        case 191:
        case 192:
        case 193:
        case 194:
        case 195:
        case 196:
        case 197:
        case 198:
        case 199:
        case 200:
        case 201:
        case 202:
        case 203:
        case 204:
        case 205:
            return 0;

        case 206:
            return 162 + rand() % 7;  // dec60
        case 207:
            return 169 + rand() % 7;  // dec61
        case 208:
            return 176 + rand() % 7;  // dec62
        case 209:
            return 183 + rand() % 7;  // dec63
        case 210:
            return 150;  // dec64 "Magic pedestal"
        case 211:
            return 151;  // dec65 "Magic pedestal"
        case 212:
            return 152;  // dec66 "Magic pedestal"
        case 213:
            return 153;  // dec67 "Magic pedestal"
        case 214:
            return 154;  // dec68 "Magic pedestal"
        case 215:
            return 155;  // dec69 "Magic pedestal"
        case 216:
            return 156;  // dec70 "Magic pedestal"
        case 217:
            return 157;  // dec71 "Magic pedestal"
        case 218:
            return 158;  // dec72 "Magic pedestal"
        case 219:
            return 159;  // dec73 "Magic pedestal"
        case 220:
            return 160;  // dec74 "Magic pedestal"
        case 221:
            return 161;  // dec75 "Magic pedestal"

        case 222:
        case 223:
        case 224:
        case 225:
        case 226:
        case 227:
            return 0;

        default:
            Error("Invalid DecorationDescID: %u", uDecorationDescID);
    }
}

//----- (0047A825) --------------------------------------------------------
bool LevelDecoration::IsObeliskChestActive() {
    // bool v1; // ebx@1
    // LevelDecoration *v2; // edi@1

    static const std::array<const PARTY_QUEST_BITS, 14> aObeliskQuests = {{
        PARTY_QUEST_OBELISK_HARMONDALE,
        PARTY_QUEST_OBELISK_ERATHIA,
        PARTY_QUEST_OBELISK_TULAREAN_FOREST,
        PARTY_QUEST_OBELISK_DEYJA,
        PARTY_QUEST_OBELISK_BRACADA_DESERT,
        PARTY_QUEST_OBELISK_CELESTE,
        PARTY_QUEST_OBELISK_THE_PIT,
        PARTY_QUEST_OBELISK_EVENMORN_ISLAND,
        PARTY_QUEST_OBELISK_MOUNT_NIGHON,
        PARTY_QUEST_OBELISK_BARROW_DOWNS,
        PARTY_QUEST_OBELISK_LAND_OF_THE_GIANTS,
        PARTY_QUEST_OBELISK_TATALIA,
        PARTY_QUEST_OBELISK_AVLEE,
        PARTY_QUEST_OBELISK_STONE_CITY
    }};

    if (pParty->uCurrentHour == 0 &&
        !TestPartyQuestBit(PARTY_QUEST_OBELISK_TREASURE_FOUND) &&
        std::all_of(aObeliskQuests.begin(), aObeliskQuests.end(),
                    TestPartyQuestBit)) {
        this->uFlags &= ~LEVEL_DECORATION_INVISIBLE;
        return true;
    }

    this->uFlags |= LEVEL_DECORATION_INVISIBLE;
    return false;
}

//----- (0044C2F4) --------------------------------------------------------
bool LevelDecoration::IsInteractive() {
    switch (uDecorationDescID) {
        case 4:    // trash pile
        case 5:    // campfire
        case 6:    // cauldron
        case 11:   // fruit plate
        case 13:   // trash pile
        case 14:   // dirt
        case 24:   // keg
        case 184:  // fire
        case 187:  // fire
        case 190:  // fire
            return true;
    }

    if (uDecorationDescID >= 206 &&
        uDecorationDescID <= 209)  // lighthouse fire
        return true;
    if (uDecorationDescID >= 210 && uDecorationDescID <= 221)  // magic pedestal
        return true;

    return false;
}

#pragma pack(push, 1)
struct LevelDecoration_mm7 {
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
#pragma pack(pop)

char *LevelDecorationsSerialize(char *pData) {
}

char *LevelDecorationsDeserialize(char *pData) {
    uint32_t uNumLevelDecorations;
    memcpy(&uNumLevelDecorations, pData, 4);
    pData += 4;

    LevelDecoration_mm7 *decorations = (LevelDecoration_mm7*)pData;
    char *names = pData + (uNumLevelDecorations * sizeof(LevelDecoration_mm7));
    for (size_t i = 0; i < uNumLevelDecorations; i++) {
        pData += sizeof(LevelDecoration_mm7);
        names += 32;
    }

    return names;
}

LevelDecoration::LevelDecoration(struct LevelDecoration_mm7 *pDecoration, const String &pDescName) {
    uDecorationDescID = pDecorationList->GetDecorIdByName(pDescName.c_str());
    uFlags = pDecoration->uFlags;
    vPosition = pDecoration->vPosition;
    field_10_y_rot = pDecoration->field_10_y_rot;
    uCog = pDecoration->uCog;
    uEventID = pDecoration->uEventID;
    uTriggerRange = pDecoration->uTriggerRange;
    field_1A = pDecoration->field_1A;
    _idx_in_stru123 = pDecoration->_idx_in_stru123;
    field_1E = pDecoration->field_1E;
}

void SetDecorationSprite(uint16_t uCog, bool bHide, const char *pFileName) {
    for (LevelDecoration &decoration : pLevelDecorations) {
        if (decoration.uCog == uCog) {
            if (pFileName && strcmp(pFileName, "0")) {
                decoration.uDecorationDescID = pDecorationList->GetDecorIdByName(pFileName);
                pDecorationList->InitializeDecorationSprite(decoration.uDecorationDescID);
            }

            if (bHide)
                decoration.uFlags &= ~LEVEL_DECORATION_INVISIBLE;
            else
                decoration.uFlags |= LEVEL_DECORATION_INVISIBLE;

            pParty->uFlags |= 2;
        }
    }
}
