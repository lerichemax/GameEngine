#pragma once
#include "Observer.h"

namespace empire {
	class TextRendererComponent;
}

enum class VersusGameEvent
{
	Player1Died,
	Player2Died = 30,
	PyramidCompleted,
	CoilyTransform = 20,
	GameOver,
};

class VersusGameManager final : public empire::Observer
{
public:
	VersusGameManager(empire::TextRendererComponent* pRoundText, empire::TextRendererComponent* pText1,
		empire::TextRendererComponent* pText2, unsigned int nbrRounds);
	void Notify(empire::GameObject* object, int event) override;

private:
	empire::TextRendererComponent* m_pRoundText;
	empire::TextRendererComponent* m_pTextP1;
	empire::TextRendererComponent* m_pTextP2;

	unsigned int m_NbrRounds;
	unsigned int m_CurrentRound;
};
