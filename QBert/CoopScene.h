#pragma once
#include "Scene.h"

class CoopScene final : public empire::Scene
{
public:
	CoopScene();
	void Initialize() override;
};
