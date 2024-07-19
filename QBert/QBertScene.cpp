#include "PCH.h"
#include "QBertScene.h"

#include "GameObject.h"
#include "InputManager.h"
#include "PauseGameCommand.h"
#include "TextRendererComponent.h"
#include "Timer.h"

QBertScene::QBertScene(std::string const& name, Level startLevel)
	: Scene(name),
	m_Level(startLevel),
	m_pQbert(nullptr),
	m_pPyramid(nullptr),
	m_pPauseMenu(nullptr),
	m_pGameOverMenu(nullptr),
	m_pEnemyManagers(),
	m_bIsPaused(false)
{
}

void QBertScene::CustomOnActivate()
{
	ResetGame();
}

void QBertScene::EndGame(bool isWin) const
{
	if (isWin)
	{
		m_pGameOverMenu->GetComponentInChildren<ECS_TextRendererComponent>()->m_Text = "You Win!";
	}
	else
	{
		m_pGameOverMenu->GetComponentInChildren<ECS_TextRendererComponent>()->m_Text = "Game Over";
	}
	m_pGameOverMenu->SetActive(true);
}

