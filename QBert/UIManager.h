#pragma once
#include "BehaviourComponent.h"

struct ECS_TextRendererComponent;
class UIManager : public BehaviourComponent
{
public:
	UIManager() = default;
	void Start() override;

	std::shared_ptr<ECS_TextRendererComponent> m_pP1PointsCounter;
	std::shared_ptr<ECS_TextRendererComponent> m_P1LivesCounter;

private:
	std::shared_ptr<GameObject> m_pPauseMenu;
	std::shared_ptr<GameObject> m_pGameOverMenu;

};