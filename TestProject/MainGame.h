#pragma once
#include "NapoleonEngine.h"

class MainGame final : public NapoleonEngine
{
public:
	MainGame();

	void InitGame() const override;
	void CreatePrefabs() const override;
};
