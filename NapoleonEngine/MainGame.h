#pragma once
#include "Minigin.h"

class MainGame final : public dae::Minigin
{
public:
	MainGame() : Minigin() {}

	void LoadGame() const override;
};

