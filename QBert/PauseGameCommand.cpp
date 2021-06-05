#include "PCH.h"
#include  "PauseGameCommand.h"

#include "GameObject.h"
#include "InputManager.h"
#include "QBertScene.h"
#include "Timer.h"

PauseGameCommand::PauseGameCommand(QBertScene* pScene, GameObject* pPauseMenu)
	:Command(),
	m_pScene(pScene),
	m_pPauseMenu(pPauseMenu)
{
	
}

PauseGameCommand::PauseGameCommand(PauseGameCommand const& other)
	:m_pPauseMenu(other.m_pPauseMenu)
{
	
}
void PauseGameCommand::Execute()
{
	if (!m_pScene->IsActive())
	{
		return;
	}
	
	if (m_pScene->IsPaused())
	{
		empire::Timer::GetInstance().SetTimeScale(1);
		m_pPauseMenu->SetActive(false);
	}
	else
	{
		empire::Timer::GetInstance().SetTimeScale(0);
		m_pPauseMenu->SetActive(true);
	}
	m_pScene->SetIsPaused(!m_pScene->IsPaused());
}
