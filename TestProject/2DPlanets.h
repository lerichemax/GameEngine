#pragma once
#include "Scene.h"

class Planets final : public Scene
{
public:
	Planets();

	void Initialize() override;
};