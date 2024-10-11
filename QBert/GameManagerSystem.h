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

enum class GameMode {
	Normal,
	Coop,
	Versus
};


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
	GameManager(CoilyManager* pCm, WrongWayManager* pWWm, SlickSamManager* pSSm, GameObject* pGameOver, unsigned int nbrPlayers = 1);
	
	void Notify(GameObject* object, int event);
	void SetNbrPlayers(unsigned int nbr) { m_NbrPlayers = nbr; }

private:

	CoilyManager* m_pCManager;
	SlickSamManager* m_pSSManager;
	WrongWayManager* m_pWWManager;
	
	GameObject* m_pGameOver;
	
	unsigned int m_NbrPlayers;
	unsigned int m_NbrDeadPlayers;
};

class GameManagerSystem : public System
{
public:
	GameManagerSystem();

	void Serialize(StreamWriter& writer) const override;

	void TogglePause();

	void SetGameMode(GameMode mode);

	void SetSignature() const override;

	EventHandler<GameManagerSystem, bool> OnGamePaused;
	EventHandler<GameManagerSystem> OnGameEnded;
	EventHandler<GameManagerSystem> OnGameOver;

protected:
	void Start() override;
	void Update() override;

private:
	QBertSystem* m_pQbert;
	PyramidSystem* m_pPyramid;

	Level m_Level;

	GameMode m_GameMode;

	bool m_IsPaused;

	int m_NbrPlayerDead;

	void ResetGame();
	void HandleEndGame();
};