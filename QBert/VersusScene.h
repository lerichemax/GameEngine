#pragma once
#include "QBertScene.h"

struct TextRendererComponent;
class Coily;
class VersusScene final : public QBertScene
{
public:
	VersusScene();


	void ResetGame() override;
	void ResetScene(Level newLevel) override;

private:
	Coily* m_pCoilyPlayer;
	TextRendererComponent* m_pRoundText;
	TextRendererComponent* m_pTextP1;
	TextRendererComponent* m_pTextP2;

	void Initialize() override;
	void DeclareInput()override;
};