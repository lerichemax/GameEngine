#pragma once
#include "Scene.h"

class MainMenuScene final : public empire::Scene
{
public:
	MainMenuScene();
	void Initialize() override;
	void CustomOnActivate() override;
};