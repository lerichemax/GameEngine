#pragma once
#include "System.h"
#include "Event.h"

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
class CharacterPoint;
struct CharacterLives;
class QBertSystem;
class GameManagerSystem : public System
{
public:
	GameManagerSystem();

	void Serialize(StreamWriter& writer) const override;

	void TogglePause();

	void SetGameMode(GameMode mode);

	void SetSignature() const override;

	EventHandler<GameManagerSystem, bool> OnGamePaused;
	EventHandler<GameManagerSystem, std::string const&> OnGameEnded;
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
	void SubscribeLifeSystem();
	void HandleEndRound(bool bPlayer1Wins);
	void ResetVersusCoily(Entity entity);
};