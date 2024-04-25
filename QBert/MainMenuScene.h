#pragma once
#include "Scene.h"

class MainMenuScene final : public Scene
{
public:
	MainMenuScene();

	void CustomOnActivate() override;

private:
	void Initialize() override;
	void DeclareInput() override{}
};