#pragma once

#include <vector>

#include "Engine/Objects/Items.h"
#include "Engine/Strings.h"

enum CHEST_FLAGS {
    CHEST_TRAPPED = 0x1,
    CHEST_ITEMS_PLACED = 0x2,
    CHEST_OPENED = 0x4,
};

struct ChestDesc {
    explicit ChestDesc(struct ChestDesc_mm7 *pChest);

    String sName;
    unsigned int uWidth;
    unsigned int uHeight;
    unsigned int uTextureID;
};

class ChestList {
 public:
    ChestList() {}

    void FromFile(void *data_mm6, void *data_mm7, void *data_mm8);

 public:
    std::vector<ChestDesc> vChests;
};

class Chest {
  public:
    void ToggleFlag(uint16_t uFlag, unsigned int bToggle);
    bool Trapped() const { return (uFlags & CHEST_TRAPPED) != 0; }
    bool Open();
    int ItemsCount() const;
    bool CanPlaceItemAt(int test_cell_position, int item_id) const;
    unsigned int GetBitmapID() const { return uChestBitmapID; }
    int GetInventoryIndex(unsigned int pos) const { return pInventoryIndices[pos]; }
    struct ItemGen *GetItem(unsigned int pos) { return &igChestItems[pos]; }
    int PutItemInChest(int position, ItemGen *put_item);
    void RemoveItemAt(unsigned int pos);
    void GenerateItems(MapInfo *pMapInfo);

    void Serialize(void *pData);
    void Deserialize(void *pData);

  protected:
    bool Initialized() const { return (uFlags & CHEST_ITEMS_PLACED) != 0; }
    void SetInitialized(bool b) {
        if (b)
            uFlags |= CHEST_ITEMS_PLACED;
        else
            uFlags &= ~CHEST_ITEMS_PLACED;
    }

    void PlaceItemAt(unsigned int put_cell_pos, unsigned int uItemIdx);
    void PlaceItems();

    unsigned int uFlags;
    unsigned int uChestBitmapID;
    struct ItemGen igChestItems[140];
    int pInventoryIndices[140];
};

class Chests {
  public:
    static bool CanPlaceItemAt(int test_cell_position, int item_id, int uChestID);
    static int CountChestItems(int uChestID);
    static int PutItemInChest(int position, ItemGen *put_item, int uChestID);
    static void ToggleFlag(int uChestID, uint16_t uFlag, unsigned int bToggle);
    static void GenerateItemsInChest();
    static unsigned int GetBitmapID(int uChestID);

    static char *Serialize(char *pData);
    static char *Deserialize(char *pData);

    static ChestList *pChestList;
    static std::vector<Chest> vChests;
};
