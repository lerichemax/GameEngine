#pragma once
#include "Observer.h"


class TextRendererComponent;

enum class VersusGameEvent
{
	Player1Died = 9,
	Player2Died = 10,
	PyramidCompleted = 8,
	CoilyTransform = 11,
	GameOver = 4,
	IncreasePoints = 12
};

class QBert;
class Coily;
class Character;
class Pyramid;
class CharacterPoint;
class VersusGameManager final : public Observer
{
public:
	VersusGameManager(TextRendererComponent* pRoundText, TextRendererComponent* pText1,
		TextRendererComponent* pText2, CharacterPoint* pP1, CharacterPoint* pP2, Pyramid* pPyramid, 
		GameObject* pGameOver, unsigned int nbrRounds);
	void Notify(GameObject* object, int event) override;

private:
	TextRendererComponent* m_pRoundText;
	TextRendererComponent* m_pTextP1;
	TextRendererComponent* m_pTextP2;

	CharacterPoint* m_pP1;
	CharacterPoint* m_pP2;
	Pyramid* m_pPyramid;
	GameObject* m_pGameOver;
	
	unsigned int m_NbrRounds;
	unsigned int m_CurrentRound;

	void UpdateRound();
	void UpdateCharacterPoint(CharacterPoint* pCharacterPoints);
	void CheckVictory() const;
};
