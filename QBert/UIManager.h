#pragma once
#include "BehaviourComponent.h"

struct TextRendererComponent;
class UIManager : public BehaviourComponent
{
public:
	UIManager() = default;
	void Start() override;

	void SetP1PointsCounter(std::shared_ptr<TextRendererComponent> p1Points);
	void SetP1LivesCounter(std::shared_ptr<TextRendererComponent> p1Lives);

	void SetPauseMenu(std::shared_ptr<GameObject> pauseMenu);
	void SetGameOverMenu(std::shared_ptr<GameObject> gameOverMenu);

private:
	std::shared_ptr<TextRendererComponent> m_pP1PointsCounter;
	std::shared_ptr<TextRendererComponent> m_P1LivesCounter;

	std::shared_ptr<GameObject> m_pPauseMenu;
	std::shared_ptr<GameObject> m_pGameOverMenu;
};