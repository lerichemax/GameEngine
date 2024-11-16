#pragma once
#include "NapoleonEngine.h"

class MainGame final : public NapoleonEngine
{
public:
	MainGame();

	void InitGame() const override;
	void CreatePrefabs() const override;

protected:
	void RegisterComponentsToFactory_Imp(Factory<Component>& factory) const override;
};