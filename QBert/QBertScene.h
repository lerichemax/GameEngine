#pragma once
#include "Scene.h"

enum class Level
{
	Level1,
	Level2,
	Level3
};

class QBert;
class Pyramid;
class EnemyManager;
class QBertScene final : public empire::Scene
{
public:
	QBertScene(Level startLevel = Level::Level1);
	~QBertScene() = default;

	void Initialize() override;
	Level GetLevel()const { return m_Level; }
	void ResetScene(Level newLevel);
	void ResetGame();

private:
	Level m_Level;
	QBert* m_pQbert;
	Pyramid* m_pPyramid;
	EnemyManager* m_pManager;
};
