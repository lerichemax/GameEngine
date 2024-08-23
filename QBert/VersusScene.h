#pragma once
#include "QBertScene.h"

class TextRendererComponent;
class Coily;
class VersusScene final : public QBertScene
{
public:
	VersusScene();

	void ResetScene(Level newLevel) override;

private:
	Coily* m_pCoilyPlayer;
	TextRendererComponent* m_pRoundText;
	TextRendererComponent* m_pTextP1;
	TextRendererComponent* m_pTextP2;

	void Initialize() override;
	void DeclareInput()override;
};