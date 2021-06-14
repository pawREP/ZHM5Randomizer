#pragma once
#include "Version.h"

// TODO: The naming here is a bit all over the place
class GameOffsets {
private:
    struct Offsets {
        void* pPushItem0;
        void* pPushItem1;
        void* pPushWorldInventoryDetour;
        void* pPushNPCInventoryDetour;
        void* pPushHeroInventoryDetour;
        void* pPushStashInventoryDetour;
        void** pZEntitySceneContext_LoadScene;
    } offsets;

    GameOffsets();

public:
    static const GameOffsets* instance();

    void* getPushItem0() const;
    void* getPushItem1() const;
    void* getPushWorldInventoryDetour() const;
    void* getPushNPCInventoryDetour() const;
    void* getPushHeroInventoryDetour() const;
    void* getPushStashInventoryDetour() const;
    void** getZEntitySceneContext_LoadScene() const;
};
