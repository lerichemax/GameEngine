#pragma once
#include "Scene.h"

class MainMenuScene final : public Scene
{
public:
	MainMenuScene();

private:
	void Initialize() override;
	void DeclareInput() override{}
};