#pragma once

class ChestUI {
  public:
    static bool Open(int uChestID);
    static bool WritePointedObjectStatusString();
    static void OnChestLeftClick();
    static void GrabItem(bool all = false);
    static void RemoveItemAtChestIndex(int index);
};
