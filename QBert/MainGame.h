#pragma once
#include "NapoleonEngine.h"

class MainGame final : public empire::NapoleonEngine
{
public:
	MainGame() : NapoleonEngine() {}

	void LoadGame() const override;
	void Cleanup() override;
};