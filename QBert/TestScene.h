#pragma once
#include "Scene.h"

class TestScene final : public Scene
{
public:
	TestScene();

private:
	void Initialize() override;
	void DeclareInput() override;
};