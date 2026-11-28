#pragma once
#include "NapoleonEngine.h"

class MainGame final : public NapoleonEngine
{
public:
	MainGame();

	void RegisterScenes() const override;
	void CreatePrefabs(std::shared_ptr<PrefabsManager> pPrefabManager) const override;
};