#pragma once
#include "Scene.h"

class CoopScene  final : public Scene
{
public:
	CoopScene();

protected:
	void Initialize() override;
};
