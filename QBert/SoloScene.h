#pragma once
#include "Scene.h"

class SoloScene final : public Scene
{
public:
	SoloScene();

protected:
	void Initialize() override;
};
