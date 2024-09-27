#pragma once

#include "System.h"
#include "Event.h"

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
class PyramidSystem;
class CoilyManager;
class SlickSamManager;
class WrongWayManager;
class QBert;
class CharacterPoint;
struct CharacterLives;
class QBertSystem;
class GameManager final
{
public:
	GameManager(TextRendererComponent* pP1Points, TextRendererComponent* pP2Points, 
		TextRendererComponent* pP1Lives, TextRendererComponent* pP2Lives, 
		CoilyManager* pCm, WrongWayManager* pWWm, SlickSamManager* pSSm, GameObject* pGameOver, unsigned int nbrPlayers = 1);
	
	~GameManager() = default;
	void Notify(GameObject* object, int event);
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

class GameManagerSystem : public System // cahnge name later
{
public:
	GameManagerSystem();

	void Serialize(StreamWriter& writer) const override;

	void TogglePause();

	void SetSignature() const override;

	EventHandler<GameManagerSystem, bool> OnGamePaused;
	EventHandler<GameManagerSystem> OnGameEnded;

protected:
	void Start() override;
	void Update() override;

private:
	QBertSystem* m_pQbert;
	PyramidSystem* m_pPyramid;

	Level m_Level;

	bool m_IsPaused;

	void ResetGame();
	void HandleEndGame();
};