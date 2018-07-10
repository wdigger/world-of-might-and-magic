#include "Engine/Objects/Chest.h"

#include "GUI/GUIWindow.h"

using EngineIoc = Engine_::IocContainer;

// should be injected in Chest but struct size cant be altered
static Mouse *mouse = EngineIoc::ResolveMouse();

ChestList *Chests::pChestList;
std::vector<Chest> Chests::vChests;

extern int pChestWidthsByType[8];
extern int pChestHeightsByType[8];
extern int pChestPixelOffsetX[8];
extern int pChestPixelOffsetY[8];

bool Chest::Open() {
    if (!Initialized()) {
        PlaceItems();
    }

    return true;
}

bool Chest::CanPlaceItemAt(int test_cell_position, int item_id) const {
    unsigned int chest_cell_heght = pChestHeightsByType[uChestBitmapID];
    unsigned int chest_cell_width = pChestWidthsByType[uChestBitmapID];

    auto img = assets->GetImage_ColorKey(pItemsTable->pItems[item_id].pIconName, 0x7FF);
    unsigned int slot_width = GetSizeInInventorySlots(img->GetWidth());
    unsigned int slot_height = GetSizeInInventorySlots(img->GetHeight());

    Assert(slot_height > 0 && slot_width > 0, "Items should have nonzero dimensions");
    if ((slot_width + test_cell_position % chest_cell_width <= chest_cell_width) &&
        (slot_height + test_cell_position / chest_cell_width <= chest_cell_heght)) {
        for (unsigned int x = 0; x < slot_width; x++) {
            for (unsigned int y = 0; y < slot_height; y++) {
                if (pInventoryIndices[y * chest_cell_width + x + test_cell_position] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

bool Chests::CanPlaceItemAt(int test_cell_position, int item_id, int uChestID) {
    return vChests[uChestID].CanPlaceItemAt(test_cell_position, item_id);
}

int Chest::ItemsCount() const {
    // this returns first free slot rather than an actual count
    int item_count = 0;
    int max_items = pChestWidthsByType[uChestBitmapID] * pChestHeightsByType[uChestBitmapID];

    if (max_items <= 0) {
        item_count = -1;
    } else {
        while (igChestItems[item_count].uItemID) {
            ++item_count;
            if (item_count >= max_items) {
                item_count = -1;
                break;
            }
        }
    }
    return item_count;
}

int Chests::CountChestItems(int uChestID) {
    return vChests[uChestID].ItemsCount();
}

int Chest::PutItemInChest(int position, ItemGen *put_item) {
    int item_in_chest_count = ItemsCount();
    int test_pos = 0;

    int max_size = pChestWidthsByType[uChestBitmapID] * pChestHeightsByType[uChestBitmapID];
    int chest_width = pChestWidthsByType[uChestBitmapID];

    if (item_in_chest_count == -1) return 0;

    if (position != -1) {
        if (CanPlaceItemAt(position, put_item->uItemID)) {
            test_pos = position;
        } else {
            position = -1;  // try another position?? is this the right behavior
        }
    }

    if (position == -1) {  // no position specified
        for (int _i = 0; _i < max_size; _i++) {
            if (Chests::CanPlaceItemAt(_i, put_item->uItemID, pChestWindow->par1C)) {
                test_pos = _i;  // found somewhere to place item
                break;
            }
        }

        if (test_pos == max_size) {  // limits check no room
            if (uActiveCharacter) {
                pPlayers[uActiveCharacter]->PlaySound(SPEECH_NoRoom, 0);
            }
            return 0;
        }
    }

    Image *texture = assets->GetImage_ColorKey(put_item->GetIconName(), 0x7FF);
    unsigned int slot_width = GetSizeInInventorySlots(texture->GetWidth());
    unsigned int slot_height = GetSizeInInventorySlots(texture->GetHeight());

    Assert(slot_height > 0 && slot_width > 0, "Items should have nonzero dimensions");

    // set inventory indices - memset was eratic??
    for (unsigned int x = 0; x < slot_width; x++) {
        for (unsigned int y = 0; y < slot_height; y++) {
            pInventoryIndices[y * chest_width + x + test_pos] = (-1 - test_pos);
        }
    }

    pInventoryIndices[test_pos] = item_in_chest_count + 1;
    igChestItems[item_in_chest_count] = *put_item;

    return (test_pos + 1);
}

int Chests::PutItemInChest(int position, ItemGen *put_item, int uChestID) {
    return vChests[uChestID].PutItemInChest(position, put_item);
}

void Chest::PlaceItemAt(unsigned int put_cell_pos, unsigned int item_at_cell) {
    int uItemID = igChestItems[item_at_cell].uItemID;
    pItemsTable->SetSpecialBonus(&igChestItems[item_at_cell]);
    if (uItemID >= 135 && uItemID <= 159 &&
        !igChestItems[item_at_cell].uNumCharges) {
        int v6 = rand() % 21 + 10;
        igChestItems[item_at_cell].uNumCharges = v6;
        igChestItems[item_at_cell].uMaxCharges = v6;
    }

    auto img = assets->GetImage_Alpha(pItemsTable->pItems[uItemID].pIconName);

    int v9 = img->GetWidth();
    if (v9 < 14) v9 = 14;
    unsigned int texture_cell_width = ((v9 - 14) >> 5) + 1;
    int v10 = img->GetHeight();
    if (v10 < 14) v10 = 14;
    unsigned int textute_cell_height = ((v10 - 14) >> 5) + 1;

    int chest_cell_width = pChestWidthsByType[uChestBitmapID];
    int chest_cell_row_pos = 0;
    for (unsigned int i = 0; i < textute_cell_height; ++i) {
        for (unsigned int j = 0; j < texture_cell_width; ++j) {
            pInventoryIndices[put_cell_pos + chest_cell_row_pos + j] = -((int)put_cell_pos + 1);
        }
        chest_cell_row_pos += chest_cell_width;
    }
    pInventoryIndices[put_cell_pos] = item_at_cell + 1;
}

void Chest::PlaceItems() {
    char chest_cells_map[144];

    int uChestArea = pChestWidthsByType[uChestBitmapID] * pChestHeightsByType[uChestBitmapID];
    memset(chest_cells_map, 0, 144);
    // fill cell map at random positions
    for (int items_counter = 0; items_counter < uChestArea; ++items_counter) {
        // get random position in chest
        int random_chest_pos = 0;
        do {
            random_chest_pos = (uint8_t)rand();
        } while (random_chest_pos >= uChestArea);
        // if this pos occupied move to next
        while (chest_cells_map[random_chest_pos]) {
            ++random_chest_pos;
            if (random_chest_pos == uChestArea) random_chest_pos = 0;
        }
        chest_cells_map[random_chest_pos] = items_counter;
    }

    for (int items_counter = 0; items_counter < uChestArea; ++items_counter) {
        int chest_item_id = igChestItems[items_counter].uItemID;
        if (chest_item_id) {
            int test_position = 0;
            while (!CanPlaceItemAt((uint8_t)chest_cells_map[test_position], chest_item_id)) {
                ++test_position;
                if (test_position >= uChestArea) break;
            }
            if (test_position < uChestArea) {
                PlaceItemAt((uint8_t)chest_cells_map[test_position], items_counter);
                if (uFlags & CHEST_OPENED) {
                    igChestItems[items_counter].SetIdentified();
                }
            }
        }
    }
    SetInitialized(true);
}

void Chest::ToggleFlag(uint16_t uFlag, unsigned int bToggle) {
    if (bToggle)
        uFlags |= uFlag;
    else
        uFlags &= ~uFlag;
}

void Chests::ToggleFlag(int uChestID, uint16_t uFlag, unsigned int bToggle) {
    if (uChestID > 19) {
        return;
    }

    vChests[uChestID].ToggleFlag(uFlag, bToggle);
}

unsigned int Chests::GetBitmapID(int uChestID) {
    return vChests[uChestID].GetBitmapID();
}

#pragma pack(push, 1)
struct ChestDesc_mm7 {
    char pName[32];
    char uWidth;
    char uHeight;
    int16_t uTextureID;
};
#pragma pack(pop)

ChestDesc::ChestDesc(struct ChestDesc_mm7 *pChest) {
    sName = pChest->pName;
    uWidth = pChest->uWidth;
    uHeight = pChest->uHeight;
    uTextureID = pChest->uTextureID;
}

void ChestList::FromFile(void *data_mm6, void *data_mm7, void *data_mm8) {
    static_assert(sizeof(ChestDesc_mm7) == 36, "Wrong type size");

    unsigned int num_mm6_chests = data_mm6 ? *(uint32_t*)data_mm6 : 0;
    unsigned int num_mm7_chests = data_mm7 ? *(uint32_t*)data_mm7 : 0;
    unsigned int num_mm8_chests = data_mm8 ? *(uint32_t*)data_mm8 : 0;

    assert(num_mm7_chests);
    assert(!num_mm8_chests);

    ChestDesc_mm7 *pChests = (ChestDesc_mm7*)((char*)data_mm7 + 4);
    for (unsigned int i = 0; i < num_mm7_chests; i++) {
        ChestDesc chest(pChests + i);
        vChests.push_back(chest);
    }

    pChests = (ChestDesc_mm7*)((char*)data_mm6 + 4);
    for (unsigned int i = 0; i < num_mm6_chests; i++) {
        ChestDesc chest(pChests + i);
        vChests.push_back(chest);
    }

    pChests = (ChestDesc_mm7*)((char*)data_mm8 + 4);
    for (unsigned int i = 0; i < num_mm8_chests; i++) {
        ChestDesc chest(pChests + i);
        vChests.push_back(chest);
    }
}

#pragma pack(push, 1)
struct ChestInfo {  // 0x14cc
    uint16_t uChestBitmapID;        // 0
    uint16_t uFlags;                // 2
    struct ItemGen igChestItems[140];       // 4
    int16_t pInventoryIndices[140];  // 0x13b4 why is this a short?
};
#pragma pack(pop)

void Chest::Serialize(void *pData) {
    static_assert(sizeof(ChestInfo) == 5324, "Wrong type size");

    ChestInfo *pChest = (ChestInfo*)pData;
    pChest->uChestBitmapID = uChestBitmapID;
    pChest->uFlags = uFlags;
    for (int j = 0; j < 140; j++) {
        pChest->igChestItems[j] = igChestItems[j];
    }
    for (int j = 0; j < 140; j++) {
        pChest->pInventoryIndices[j] = pInventoryIndices[j];
    }
}
void Chest::Deserialize(void *pData) {
    ChestInfo *pChest = (ChestInfo*)pData;
    uChestBitmapID = pChest->uChestBitmapID;
    uFlags = pChest->uFlags;
    for (int j = 0; j < 140; j++) {
        igChestItems[j] = pChest->igChestItems[j];
    }
    for (int j = 0; j < 140; j++) {
        pInventoryIndices[j] = pChest->pInventoryIndices[j];
    }
}

char *Chests::Serialize(char *pData) {
    uint32_t uNumChests = vChests.size();
    *(uint32_t*)pData = uNumChests;
    pData += 4;
    ChestInfo *pChests = (ChestInfo*)pData;
    for (unsigned int i = 0; i < uNumChests; i++) {
        vChests[i].Serialize(pChests);
        pChests++;
    }
    pData += sizeof(ChestInfo) * uNumChests;
    return pData;
}

char *Chests::Deserialize(char *pData) {
    vChests.clear();
    uint32_t uNumChests = 0;
    uNumChests = *(uint32_t*)pData;
    pData += 4;
    ChestInfo *pChests = (ChestInfo*)pData;
    for (unsigned int i = 0; i < uNumChests; i++) {
        Chest newChest;
        newChest.Deserialize(pChests);
        vChests.push_back(newChest);
        pChests++;
    }
    pData += uNumChests * sizeof(ChestInfo);
    return pData;
}

void Chest::RemoveItemAt(unsigned int pos) {
    int chestindex = pInventoryIndices[pos];
    ItemGen *item_in_slot = &igChestItems[chestindex - 1];

    auto img = assets->GetImage_ColorKey(item_in_slot->GetIconName(), 0x7FF);
    unsigned int slot_width = GetSizeInInventorySlots(img->GetWidth());
    unsigned int slot_height = GetSizeInInventorySlots(img->GetHeight());

    int chestwidth = pChestWidthsByType[uChestBitmapID];

    item_in_slot->Reset();

    if (slot_width > 0) {
        // blank inventory indices - memset was eratic??
        for (unsigned int x = 0; x < slot_width; x++) {
            for (unsigned int y = 0; y < slot_height; y++) {
                pInventoryIndices[y * chestwidth + x + pos] = 0;
            }
        }
    }
}

void Chest::GenerateItems(MapInfo *pMapInfo) {
    for (int j = 0; j < 140; ++j) {
        ItemGen *currItem = &igChestItems[j];
        if (currItem->uItemID < 0) {
            int additionaItemCount = rand() % 5;  // additional items in chect
            additionaItemCount++;  // + 1 because it's the item at pChests[i].igChestItems[j] and the additional ones
            int treasureLevelBot = byte_4E8168[abs(currItem->uItemID) - 1][2 * pMapInfo->Treasure_prob];
            int treasureLevelTop = byte_4E8168[abs(currItem->uItemID) - 1][2 * pMapInfo->Treasure_prob + 1];
            int treasureLevelRange = treasureLevelTop - treasureLevelBot + 1;
            int resultTreasureLevel = treasureLevelBot + rand() % treasureLevelRange;  // treasure level
            if (resultTreasureLevel < 7) {
                for (int k = 0; k < additionaItemCount; k++) {
                    int whatToGenerateProb = rand() % 100;
                    if (whatToGenerateProb < 20) {
                        currItem->Reset();
                    } else if (whatToGenerateProb < 60) {  // generate gold
                        int goldAmount = 0;
                        currItem->Reset();
                        switch (resultTreasureLevel) {
                        case 1:
                            goldAmount = rand() % 51 + 50;
                            currItem->uItemID = ITEM_GOLD_SMALL;
                            break;
                        case 2:
                            goldAmount = rand() % 101 + 100;
                            currItem->uItemID = ITEM_GOLD_SMALL;
                            break;
                        case 3:
                            goldAmount = rand() % 301 + 200;
                            currItem->uItemID = ITEM_GOLD_MEDIUM;
                            break;
                        case 4:
                            goldAmount = rand() % 501 + 500;
                            currItem->uItemID = ITEM_GOLD_MEDIUM;
                            break;
                        case 5:
                            goldAmount = rand() % 1001 + 1000;
                            currItem->uItemID = ITEM_GOLD_LARGE;
                            break;
                        case 6:
                            goldAmount = rand() % 3001 + 2000;
                            currItem->uItemID = ITEM_GOLD_LARGE;
                            break;
                        }
                        currItem->SetIdentified();
                        currItem->special_enchantment = (ITEM_ENCHANTMENT)goldAmount;
                    } else {
                        pItemsTable->GenerateItem(resultTreasureLevel, 0, currItem);
                    }

                    for (int m = 0; m < 140; m++) {
                        if (igChestItems[m].uItemID == ITEM_NULL) {
                            currItem = &igChestItems[m];
                            break;
                        }
                    }
                }
            } else {
                currItem->GenerateArtifact();
            }
        }
    }
}

void Chests::GenerateItemsInChest() {
    unsigned int mapType = pMapStats->GetMapInfo(pCurrentMapName);
    MapInfo *currMapInfo = &pMapStats->pInfos[mapType];
    for (int i = 1; i < 20; ++i) {
        Chest *pChest = &vChests[i];
        pChest->GenerateItems(currMapInfo);
    }
}
