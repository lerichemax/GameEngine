#pragma once
#include "BehaviourComponent.h"

class TextRendererComponent;
class UIManager : public BehaviourComponent
{
public:
	UIManager() = default;
	~UIManager()
	{

	}
	void Start() override;

	void Serialize(StreamWriter& writer) const override;

	void SetP1PointsCounter(TextRendererComponent* const p1Points);
	void SetP1LivesCounter(TextRendererComponent* const p1Lives);

	void SetPauseMenu(GameObject* const pauseMenu);
	void SetGameOverMenu(GameObject* const gameOverMenu);

private:
	TextRendererComponent* m_pP1PointsCounter;
	TextRendererComponent* m_P1LivesCounter;

	GameObject* m_pPauseMenu;
	GameObject* m_pGameOverMenu;
};