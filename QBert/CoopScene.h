#pragma once
#include "Scene.h"

class CoopScene  final : public Scene
{
public:
	CoopScene();

	//void ResetScene(Level newLevel);

protected:
	void Initialize() override;
};
