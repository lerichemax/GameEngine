#pragma once
#include "Observer.h"

enum class GameEvent
{
	PlayerDied,
	IncreasePoints,
	JumpOnQube,
	JumpOnDisk,
	JumpOffDisk,
	GameOver,
	CoilyDies,
	SlickSamDies,
	WrongWayDies,
};

namespace empire
{
	class TextRendererComponent;
}


class Pyramid;
class EnemyManager;
class QBert;
class CharacterPoint;
class CharacterLives;
class GameManager final : public empire::Observer
{
public:
	GameManager(empire::TextRendererComponent* pP1Points, empire::TextRendererComponent* pP2Points, 
		empire::TextRendererComponent* pP1Lives, empire::TextRendererComponent* pP2Lives, Pyramid* pPyramid, 
		EnemyManager* pManager, GameObject* pGameOver, unsigned int nbrPlayers = 1);
	
	~GameManager() = default;
	void Notify(empire::GameObject* object, int event) override;
	void SetNbrPlayers(unsigned int nbr) { m_NbrPlayers = nbr; }
private:
	empire::TextRendererComponent* const m_pP1PointsCounter;
	empire::TextRendererComponent* const m_P1LivesCounter;
	empire::TextRendererComponent* const m_pP2PointsCounter;
	empire::TextRendererComponent* const m_P2LivesCounter;
	
	EnemyManager* const m_pEnemyManager;
	GameObject* m_pGameOver;
	
	unsigned int m_NbrPlayers;
	unsigned int m_NbrDeadPlayers;
	
	void UpdateLivesText(CharacterLives* pLives, int playerNbr);
	void UpdatePointsText(CharacterPoint* pPoints, int playerNbr);
	
};