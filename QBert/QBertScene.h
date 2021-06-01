#pragma once
#include "Scene.h"

class QBert;
class Pyramid;
class EnemyManager;
class QBertScene : public empire::Scene
{
public:
	enum class Level
	{
		Level1,
		Level2,
		Level3
	};
	
	QBertScene(std::string const& name, Level startLevel = Level::Level1);
	~QBertScene() = default;

	virtual void Initialize() override = 0;
	Level GetLevel()const { return m_Level; }
	virtual void ResetScene(Level newLevel) = 0;
	virtual void ResetGame() = 0;
	void CustomOnActivate() override;
	
	bool IsPaused()const { return m_bIsPaused; }
	void SetIsPaused(bool isPaused) { m_bIsPaused = isPaused; }
	void EndGame(bool isWin);
protected:
	Level m_Level;
	QBert* m_pQbert;
	Pyramid* m_pPyramid;
	EnemyManager* m_pManager;
	GameObject* m_pPauseMenu;
	GameObject* m_pGameOverMenu;

private:
	bool m_bIsPaused;
};
