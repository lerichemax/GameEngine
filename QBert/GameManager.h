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
class CoilyManager;
class SlickSamManager;
class WrongWayManager;
class QBert;
class CharacterPoint;
class CharacterLives;
class GameManager final : public empire::Observer
{
public:
	GameManager(empire::TextRendererComponent* pP1Points, empire::TextRendererComponent* pP2Points, 
		empire::TextRendererComponent* pP1Lives, empire::TextRendererComponent* pP2Lives, 
		CoilyManager* pCm, WrongWayManager* pWWm, SlickSamManager* pSSm, GameObject* pGameOver, unsigned int nbrPlayers = 1);
	
	~GameManager() = default;
	void Notify(empire::GameObject* object, int event) override;
	void SetNbrPlayers(unsigned int nbr) { m_NbrPlayers = nbr; }
private:
	empire::TextRendererComponent* const m_pP1PointsCounter;
	empire::TextRendererComponent* const m_P1LivesCounter;
	empire::TextRendererComponent* const m_pP2PointsCounter;
	empire::TextRendererComponent* const m_P2LivesCounter;

	CoilyManager* m_pCManager;
	SlickSamManager* m_pSSManager;
	WrongWayManager* m_pWWManager;
	
	GameObject* m_pGameOver;
	
	unsigned int m_NbrPlayers;
	unsigned int m_NbrDeadPlayers;
	
	void UpdateLivesText(CharacterLives* pLives, int playerNbr);
	void UpdatePointsText(CharacterPoint* pPoints, int playerNbr);
	
};