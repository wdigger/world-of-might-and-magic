#include "Engine/Objects/ControllerChest.h"

#include "Engine/Localization.h"
#include "Engine/Party.h"
#include "Engine/Point.h"

#include "IO/Mouse.h"

#include "GUI/GUIWindow.h"
#include "GUI/UI/UIChest.h"
#include "GUI/UI/UIStatusBar.h"

#include "Media/Audio/AudioPlayer.h"

#include "Engine/Graphics/DecorationList.h"
#include "Engine/Graphics/Indoor.h"
#include "Engine/Graphics/Level/Decoration.h"
#include "Engine/Graphics/Outdoor.h"
#include "Engine/Graphics/Viewport.h"

#include "Engine/Objects/Actor.h"
#include "Engine/Objects/Chest.h"
#include "Engine/Objects/ObjectList.h"

using EngineIoc = Engine_::IocContainer;

// should be injected in Chest but struct size cant be altered
static Mouse *mouse = EngineIoc::ResolveMouse();

extern int pChestWidthsByType[8];
extern int pChestHeightsByType[8];
extern int pChestPixelOffsetX[8];
extern int pChestPixelOffsetY[8];

bool ChestUI::Open(int uChestID) {
    assert(uChestID < 20);
    if ((uChestID < 0) && (uChestID >= 20)) return false;
    Chest *chest = &Chests::vChests[uChestID];

    chest->Open();

    if (!uActiveCharacter) return false;

    bool flag_shout = false;
    unsigned int pMapID = pMapStats->GetMapInfo(pCurrentMapName);
    if (chest->Trapped() && pMapID) {
        if (pPlayers[uActiveCharacter]->GetDisarmTrap() < 2 * pMapStats->pInfos[pMapID].LockX5) {
            SPRITE_OBJECT_TYPE pSpriteID[4];
            pSpriteID[0] = SPRITE_811;
            pSpriteID[1] = SPRITE_812;
            pSpriteID[2] = SPRITE_813;
            pSpriteID[3] = SPRITE_814;
            int pRandom = rand() % 4;
            int v6 = PID_ID(EvtTargetObj);
            int pObjectX;
            int pObjectY;
            int pObjectZ;
            if (PID_TYPE(EvtTargetObj) == OBJECT_Decoration) {
                pObjectX = pLevelDecorations[v6].vPosition.x;
                pObjectY = pLevelDecorations[v6].vPosition.y;
                pObjectZ = pLevelDecorations[v6].vPosition.z +
                    (pDecorationList->GetDecoration(pLevelDecorations[v6].uDecorationDescID)->uDecorationHeight / 2);
            }
            if (PID_TYPE(EvtTargetObj) == OBJECT_BModel) {
                if (uCurrentlyLoadedLevelType == LEVEL_Outdoor) {
                    ODMFace *pODMFace = &pOutdoor->pBModels[EvtTargetObj >> 9].pFaces[(EvtTargetObj >> 3) & 0x3F];
                    pObjectX = (pODMFace->pBoundingBox.x1 + pODMFace->pBoundingBox.x2) / 2;
                    pObjectY = (pODMFace->pBoundingBox.y1 + pODMFace->pBoundingBox.y2) / 2;
                    pObjectZ = (pODMFace->pBoundingBox.z1 + pODMFace->pBoundingBox.z2) / 2;
                } else {  // Indoor
                    BLVFace *pBLVFace = &pIndoor->pFaces[v6];
                    pObjectX = (pBLVFace->pBounding.x1 + pBLVFace->pBounding.x2) / 2;
                    pObjectY = (pBLVFace->pBounding.y1 + pBLVFace->pBounding.y2) / 2;
                    pObjectZ = (pBLVFace->pBounding.z1 + pBLVFace->pBounding.z2) / 2;
                }
            }
            float dir_x = (float)(pParty->vPosition.x - pObjectX);
            float dir_y = (float)(pParty->vPosition.y - pObjectY);
            float dir_z = (float)((pParty->sEyelevel + pParty->vPosition.z) - pObjectZ);
            float length_vector = sqrt((dir_x * dir_x) + (dir_y * dir_y) + (dir_z * dir_z));
            int sRotX;
            int sRotY;
            if (length_vector <= 1.f) {
                sRotX = 0;
                sRotY = 0;
            } else {
                sRotY = (int)sqrt(dir_x * dir_x + dir_y * dir_y);
                sRotX = stru_5C6E00->Atan2((int)dir_x, (int)dir_y);
                sRotY = stru_5C6E00->Atan2((int)(dir_y * dir_y), (int)dir_z);
            }
            int pDepth = 256;
            if (length_vector < 256.f) {
                pDepth = (int)(length_vector / 4);
            }
            Vec3_int_ v;
            v.x = pObjectX;
            v.y = pObjectY;
            v.z = pObjectZ;
            Vec3_int_ pOut;
            Vec3_int_::Rotate(pDepth, sRotX, sRotY, v, &pOut.x, &pOut.z, &pOut.y);
            SpriteObject::sub_42F7EB_DropItemAt(pSpriteID[pRandom], pOut.x,
                                                pOut.z, pOut.y, 0, 1, 0, 48, 0);

            SpriteObject pSpellObject;
            pSpellObject.containing_item.Reset();
            pSpellObject.spell_skill = 0;
            pSpellObject.spell_level = 0;
            pSpellObject.spell_id = 0;
            pSpellObject.field_54 = 0;
            pSpellObject.uType = pSpriteID[pRandom];
            pSpellObject.uObjectDescID = pObjectList->ObjectIDByItemID(pSpellObject.uType);
            pSpellObject.vPosition.y = pOut.z;
            pSpellObject.vPosition.x = pOut.x;
            pSpellObject.vPosition.z = pOut.y;
            pSpellObject.uSoundID = 0;
            pSpellObject.uAttributes = 48;
            pSpellObject.uSectorID = pIndoor->GetSector(pOut.x, pOut.z, pOut.y);
            pSpellObject.uSpriteFrameID = 0;
            pSpellObject.spell_caster_pid = 0;
            pSpellObject.spell_target_pid = 0;
            pSpellObject.uFacing = 0;
            pSpellObject.Create(0, 0, 0, 0);
            pAudioPlayer->PlaySound(SOUND_fireBall, 0, 0, -1, 0, 0);
            pSpellObject.ExplosionTraps();
            chest->ToggleFlag(CHEST_TRAPPED, false);
            if (uActiveCharacter && !_A750D8_player_speech_timer &&
                !OpenedTelekinesis) {
                _A750D8_player_speech_timer = 256;
                PlayerSpeechID = SPEECH_5;
                uSpeakingCharacter = uActiveCharacter;
            }
            OpenedTelekinesis = false;
            return false;
        }
        chest->ToggleFlag(CHEST_TRAPPED, false);
        flag_shout = true;
    }
    pAudioPlayer->StopChannels(-1, -1);
    pAudioPlayer->PlaySound(SOUND_openchest0101, 0, 0, -1, 0, 0);
    if (flag_shout == true) {
        if (!OpenedTelekinesis) {
            pPlayers[uActiveCharacter]->PlaySound(SPEECH_4, 0);
        }
    }
    OpenedTelekinesis = false;
    pChestWindow = pGUIWindow_CurrentMenu = new GUIWindow_Chest(uChestID);
    return true;
}

bool ChestUI::WritePointedObjectStatusString() {
    Point pt = mouse->GetCursorPos();
    unsigned int pX = pt.x;
    unsigned int pY = pt.y;

    Chest *chest = &Chests::vChests[(int)pGUIWindow_CurrentMenu->par1C];
    unsigned int uiBitmapID = chest->GetBitmapID();

    int chestheight = pChestHeightsByType[uiBitmapID];
    int chestwidth = pChestWidthsByType[uiBitmapID];

    int inventoryYCoord = (pY - (pChestPixelOffsetY[uiBitmapID])) / 32;
    int inventoryXCoord = (pX - (pChestPixelOffsetX[uiBitmapID])) / 32;
    int invMatrixIndex = inventoryXCoord + (chestheight * inventoryYCoord);

    if (inventoryYCoord >= 0 && inventoryYCoord < chestheight &&
        inventoryXCoord >= 0 && inventoryXCoord < chestwidth) {
        int chestindex = chest->GetInventoryIndex(invMatrixIndex);
        if (chestindex < 0) {
            invMatrixIndex = (-(chestindex + 1));
            chestindex = chest->GetInventoryIndex(invMatrixIndex);
        }

        if (chestindex) {
            int itemindex = chestindex - 1;
            ItemGen *item = chest->GetItem(itemindex);

            ///////////////////////////////////////////////
            // normal picking

            GameUI_StatusBar_Set(item->GetDisplayName());
            uLastPointedObjectID = 1;
            return 1;

            ////////////////////////////////////////////////////

            // per pixel transparency check tests
            /*

            auto img = assets->GetImage_16BitColorKey(item->GetIconName(),
            0x7FF);

            int imgwidth = img->GetWidth();
            int imgheight = img->GetHeight();
            auto pixels = (signed __int32
            *)img->GetPixels(IMAGE_FORMAT_A8R8G8B8);

            Assert(pixels != nullptr, "Cannot get pixels");

            if (imgwidth < 14)
                    imgwidth = 14;

            int v12 = imgwidth - 14;
            v12 = v12 & 0xFFFFFFE0;
             int v13 = v12 + 32;

            if (imgheight < 14)
                    imgheight = 14;

            int chest_offs_x =
            42;//pChestPixelOffsetX[(int)pGUIWindow_CurrentMenu->par1C].uChestBitmapID];
            int chest_offs_y = 34; //
            pChestPixelOffsetY[(int)pGUIWindow_CurrentMenu->par1C].uChestBitmapID];

            int imgX = chest_offs_x + 32 * (invMatrixIndex % chestwidth) +
            ((signed int)(v13 - imgwidth) / 2);

            int imgY = chest_offs_y + 32 * (invMatrixIndex / chestheight) +
                    ((signed int)(((imgheight - 14) & 0xFFFFFFE0) + 32 -
            imgheight) / 2);

            int pix_chk_x = pX-imgX;
            int pix_chk_y = pY-imgY;

            if (pix_chk_x > 0 && pix_chk_x <= imgwidth && pix_chk_y > 0 &&
            pix_chk_y <= imgheight) {

                    pixels += pix_chk_x + pix_chk_y*imgwidth;

                    if (*pixels & 0xFF000000) {
                            GameUI_StatusBar_Set(item->GetDisplayName());
                            uLastPointedObjectID = 1;
                            return 1;
                    }
            }

            */
        }
    }
    return 0;
}

void ChestUI::RemoveItemAtChestIndex(int index) {
    Chest *chest = &Chests::vChests[pGUIWindow_CurrentMenu->par1C];
    chest->RemoveItemAt(index);
}

void ChestUI::OnChestLeftClick() {
    Chest *chest = &Chests::vChests[pGUIWindow_CurrentMenu->par1C];
    unsigned int uiBitmapID = chest->GetBitmapID();

    int chestheight = pChestHeightsByType[uiBitmapID];
    int chestwidth = pChestWidthsByType[uiBitmapID];

    unsigned int pX;
    unsigned int pY;
    mouse->GetClickPos(&pX, &pY);
    int inventoryYCoord = (pY - (pChestPixelOffsetY[uiBitmapID])) / 32;
    int inventoryXCoord = (pX - (pChestPixelOffsetX[uiBitmapID])) / 32;

    int invMatrixIndex = inventoryXCoord + (chestheight * inventoryYCoord);

    if (inventoryYCoord >= 0 && inventoryYCoord < chestheight &&
        inventoryXCoord >= 0 && inventoryXCoord < chestwidth) {
        if (pParty->pPickedItem.uItemID) {  // item held
            if (Chests::PutItemInChest(invMatrixIndex, &pParty->pPickedItem, pGUIWindow_CurrentMenu->par1C)) {
                mouse->RemoveHoldingItem();
            }
        } else {
            int chestindex = chest->GetInventoryIndex(invMatrixIndex);
            if (chestindex < 0) {
                invMatrixIndex = (-(chestindex + 1));
                chestindex = chest->GetInventoryIndex(invMatrixIndex);
            }

            if (chestindex) {
                int itemindex = chestindex - 1;

                if (chest->GetItem(itemindex)->GetItemEquipType() == EQUIP_GOLD) {
                    pParty->PartyFindsGold(chest->GetItem(itemindex)->special_enchantment, 0);
                    viewparams->bRedrawGameUI = 1;
                } else {
                    pParty->SetHoldingItem(chest->GetItem(itemindex));
                }

                RemoveItemAtChestIndex(invMatrixIndex);
            }
        }
    }
}

void ChestUI::GrabItem(bool all) {  // new fucntion to grab items from chest using spacebar
    if (pParty->pPickedItem.uItemID || !uActiveCharacter) {
        return;
    }

    int InventSlot;
    int grabcount = 0;
    int goldcount = 0;
    int goldamount = 0;

    Chest *chest = &Chests::vChests[pGUIWindow_CurrentMenu->par1C];

    // loop through chest pInvetoryIndices
    for (int loop = 0; loop < 140; loop++) {
        int chestindex = chest->GetInventoryIndex(loop);
        if (chestindex <= 0) continue;  // no item here

        int itemindex = chestindex - 1;
        ItemGen *chestitem = chest->GetItem(itemindex);
        if (chestitem->GetItemEquipType() == EQUIP_GOLD) {
            pParty->PartyFindsGold(chestitem->special_enchantment, 0);
            viewparams->bRedrawGameUI = 1;
            goldamount += chestitem->special_enchantment;
            goldcount++;
        } else {  // this should add item to invetory of active char - if that fails set as holding item and break
            if (uActiveCharacter && (InventSlot = pPlayers[uActiveCharacter]->AddItem(-1, chestitem->uItemID)) != 0) {  // can place
                memcpy(&pPlayers[uActiveCharacter]->pInventoryItemList[InventSlot - 1], &chestitem, 0x24u);
                grabcount++;
                GameUI_StatusBar_OnEvent(localization->FormatString(471, pItemsTable->pItems[chestitem->uItemID].pUnidentifiedName));  // You found an item (%s)!
            } else {  // no room so set as holding item
                pParty->SetHoldingItem(chestitem);
                RemoveItemAtChestIndex(loop);
                pPlayers[uActiveCharacter]->PlaySound(SPEECH_NoRoom, 0);
                break;
            }
        }
        RemoveItemAtChestIndex(loop);
        if (all == false)  // only grab 1 item
            break;
    }

    if (grabcount > 1 || goldcount > 1) {  // found items
        char out[200];
        sprintf(out, "You found %d item(s) and %d Gold!", grabcount, goldamount);
        GameUI_StatusBar_OnEvent(out);
    }
    if (grabcount == 0 && goldcount == 0) {  // nothing here
        GameUI_StatusBar_OnEvent(localization->GetString(521));
    }
}
