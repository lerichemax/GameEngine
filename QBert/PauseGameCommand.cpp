#include "PCH.h"
#include  "PauseGameCommand.h"

#include "GameObject.h"
#include "InputManager.h"
#include "GameManagerSystem.h"
#include "Timer.h"

PauseGameCommand::PauseGameCommand(GameManagerSystem* const pGameManager)
	:m_pGameManager(pGameManager)
{
}

void PauseGameCommand::Execute(GameObject const& gObject)
{
	m_pGameManager->TogglePause();
}
