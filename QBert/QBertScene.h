#pragma once
#include "Scene.h"

class QBert;
class Pyramid;
class EnemyManager;
class QBertScene : public Scene
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

	
	Level GetLevel()const { return m_Level; }
	virtual void ResetScene(Level newLevel) = 0;
	virtual void ResetGame() = 0;
	void CustomOnActivate() override;
	
	bool IsPaused()const { return m_bIsPaused; }
	void SetIsPaused(bool isPaused) { m_bIsPaused = isPaused; }
	void EndGame(bool isWin) const;

protected:
	Level m_Level;
	QBert* m_pQbert;
	Pyramid* m_pPyramid;
	std::shared_ptr<GameObject> m_pPauseMenu;
	std::shared_ptr<GameObject>m_pGameOverMenu;
	std::vector<EnemyManager*> m_pEnemyManagers;
	
	virtual void DeclareInput() override{};
private:
	bool m_bIsPaused;

	virtual void Initialize() override = 0;

};
