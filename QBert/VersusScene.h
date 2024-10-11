#pragma once
#include "Scene.h"

class TextRendererComponent;
class Coily;
class VersusScene final : public Scene
{
public:
	VersusScene();

protected:

	void Initialize() override;
};