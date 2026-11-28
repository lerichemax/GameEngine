#pragma once
#include "Scene.h"

class GameScene final : public Scene
{
public:
	GameScene();

protected:
	void Initialize() override;
};