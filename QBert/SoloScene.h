#pragma once
#include "QBertScene.h"

class SoloScene final : public QBertScene
{
public:
	SoloScene();
	void Initialize() override;

	void ResetGame() override;
	void ResetScene(Level newLevel) override;
};
