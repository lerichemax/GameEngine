#pragma once
#include "Scene.h"

class TestScene final : public empire::Scene
{
public:
	TestScene() : Scene("TestScene"){}
	void Initialize() override;
};