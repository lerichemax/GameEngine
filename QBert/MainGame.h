#pragma once
#include "NapoleonEngine.h"

class MainGame final : public NapoleonEngine
{
public:
	MainGame();

	void LoadGame() const override;
	void CreatePrefabs() const override;
};