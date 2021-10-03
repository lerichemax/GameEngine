#include "PCH.h"
#include "CharacterController.h"
#include "InputManager.h"

void CharacterController::Initialize()
{
	
}

void CharacterController::Update()
{
	if (InputManager::GetInstance().IsActionTriggered(100))
	{
		Debugger::GetInstance().Log("OK");
	}
}