#pragma once
#include "Observer.h"

#include "BehaviourComponent.h"

enum class GameEvent
{
	PlayerDied,
	IncreasePoints,
	JumpOnDisk,
	JumpOffDisk,
	GameOver,
	CoilyDies,
	SlickSamDies,
	WrongWayDies,
	PyramidCompleted
};

enum class Level
{
	Level1,
	Level2,
	Level3
};


class TextRendererComponent;
class Pyramid;
class CoilyManager;
class SlickSamManager;
class WrongWayManager;
class QBert;
class CharacterPoint;
class CharacterLives;
class GameManager final : public Observer
{
public:
	GameManager(TextRendererComponent* pP1Points, TextRendererComponent* pP2Points, 
		TextRendererComponent* pP1Lives, TextRendererComponent* pP2Lives, 
		CoilyManager* pCm, WrongWayManager* pWWm, SlickSamManager* pSSm, GameObject* pGameOver, unsigned int nbrPlayers = 1);
	
	~GameManager() = default;
	void Notify(GameObject* object, int event) override;
	void SetNbrPlayers(unsigned int nbr) { m_NbrPlayers = nbr; }

private:
	TextRendererComponent* const m_pP1PointsCounter;
	TextRendererComponent* const m_P1LivesCounter;
	TextRendererComponent* const m_pP2PointsCounter;
	TextRendererComponent* const m_P2LivesCounter;

	CoilyManager* m_pCManager;
	SlickSamManager* m_pSSManager;
	WrongWayManager* m_pWWManager;
	
	GameObject* m_pGameOver;
	
	unsigned int m_NbrPlayers;
	unsigned int m_NbrDeadPlayers;
	
	void UpdateLivesText(CharacterLives* pLives, int playerNbr);
	void UpdatePointsText(CharacterPoint* pPoints, int playerNbr);
	
};

class GameManagerBehaviour : public BehaviourComponent
{
public:
	void Serialize(StreamWriter& writer) const override;

	void PauseGame();

protected:
	void Start() override;

private:
	std::shared_ptr<QBert> m_pQbert;
	std::shared_ptr<Pyramid> m_pPyramid;

	Level m_Level;

	void ResetGame();
	void HandleEndGame();
};