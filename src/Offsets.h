#pragma once

//TODO: The naming here is a bit all over the place
class GameOffsets {
private:
	struct Offsets {
		void* pPushItem;

		void* pPushWorldInventoryDetour;
		void* pPushNPCInventoryDetour;
		void* pPushHeroInventoryDetour;
		void* pPushStashInventoryDetour;
		void** pZEntitySceneContext_LoadScene;
	} offsets;

	GameOffsets();

	enum class GameVersion {
		DX11,
		DX12,
		UNK
	};

	GameVersion getVersion() const ;

public:

	static const GameOffsets* instance();

	void* getPushItem()const;
	void* getPushWorldInventoryDetour()const;
	void* getPushNPCInventoryDetour()const;
	void* getPushHeroInventoryDetour()const;
	void* getPushStashInventoryDetour()const;
	void** getZEntitySceneContext_LoadScene()const;
};
