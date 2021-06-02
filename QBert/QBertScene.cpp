#include "PCH.h"
#include "QBertScene.h"

#include "GameObject.h"
#include "InputManager.h"
#include "PauseGameCommand.h"
#include "TextRendererComponent.h"
#include "Timer.h"


using namespace empire;

QBertScene::QBertScene(std::string const& name, Level startLevel)
	: Scene(name),
	m_Level(startLevel),
	m_pQbert(nullptr),
	m_pPyramid(nullptr),
	m_pEnemyManager(nullptr)
{
}

void QBertScene::CustomOnActivate()
{
	ResetGame();
}

void QBertScene::EndGame(bool isWin)
{
	if (isWin)
	{
		m_pGameOverMenu->GetComponentInChildren<TextRendererComponent>()->SetText("You Win!");
	}
	else
	{
		m_pGameOverMenu->GetComponentInChildren<TextRendererComponent>()->SetText("Game Over");
	}
	m_pGameOverMenu->SetActive(true);
}

void QBertScene::DeclareInput()
{
	InputManager::GetInstance().AddInputAction(100, new InputAction{ SDLK_ESCAPE, empire::KeyActionState::pressed,
	new PauseGameCommand(this, m_pPauseMenu) });

	InputManager::GetInstance().AddInputAction(101, new InputAction{ ControllerButton::Start, empire::KeyActionState::pressed,
		new PauseGameCommand(this, m_pPauseMenu), PlayerNbr::One });
}