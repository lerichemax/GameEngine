#pragma once
#include "QBertScene.h"

namespace empire
{
	class TextRendererComponent;
}

class Coily;
class VersusScene final : public QBertScene
{
public:
	VersusScene();

	void Initialize() override;

private:
	Coily* m_pCoilyPlayer;
	empire::TextRendererComponent* m_pRoundText;
	empire::TextRendererComponent* m_pTextP1;
	empire::TextRendererComponent* m_pTextP2;
	
	void ResetGame() override;
	void ResetScene(Level newLevel) override;
};