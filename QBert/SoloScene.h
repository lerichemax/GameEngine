#pragma once
#include "QBertScene.h"

class SoloScene final : public QBertScene
{
public:
	SoloScene();
	
	void ResetScene(Level newLevel) override;

private:
	void Initialize() override;
	void DeclareInput();
};
