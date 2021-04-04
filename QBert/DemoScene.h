#pragma once
#include "Scene.h"

class DemoScene final : public empire::Scene
{
public:
	DemoScene();
	~DemoScene() = default;

	void Initialize() override;
	
};
