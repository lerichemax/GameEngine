#pragma once
#include "QBertScene.h"

class SlickSamManager;
class CoilyManager;
class WrongWayManager;
class SoloScene final : public QBertScene
{
public:
	SoloScene();
	

	void ResetGame() override;
	void ResetScene(Level newLevel) override;

private:
	void Initialize() override;
	void DeclareInput() override;
};
