#pragma once
#include "Observer.h"

namespace empire {
	class TextRendererComponent;
}

enum class VersusGameEvent
{
	Player1Died = 9,
	Player2Died = 10,
	PyramidCompleted = 11,
	CoilyTransform = 12,
	GameOver = 5,
	IncreasePoints = 13
};

class QBert;
class Coily;
class Character;
class Pyramid;
class CharacterPoint;
class VersusGameManager final : public empire::Observer
{
public:
	VersusGameManager(empire::TextRendererComponent* pRoundText, empire::TextRendererComponent* pText1,
		empire::TextRendererComponent* pText2, CharacterPoint* pP1, CharacterPoint* pP2, Pyramid* pPyramid, unsigned int nbrRounds);
	void Notify(empire::GameObject* object, int event) override;

private:
	empire::TextRendererComponent* m_pRoundText;
	empire::TextRendererComponent* m_pTextP1;
	empire::TextRendererComponent* m_pTextP2;

	CharacterPoint* m_pP1;
	CharacterPoint* m_pP2;
	Pyramid* m_pPyramid;
	
	unsigned int m_NbrRounds;
	unsigned int m_CurrentRound;

	void UpdateRound(empire::GameObject* pObject);
	void UpdateCharacterPoint(CharacterPoint* pCharacterPoints);
};
