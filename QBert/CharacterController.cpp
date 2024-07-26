#include "PCH.h"
#include "CharacterController.h"

#include "CharacterMovement.h"

#include "MoveCommand.h"

#include "InputManager.h"

void CharacterController::Initialize()
{
	m_pMover = GetGameObject()->GetComponent<CharacterMovement>();
}  
  
void CharacterController::Start()
{
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_d , KeyActionState::released,
		new MoveCommand(ConnectionDirection::downRight, m_pMover) });
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_s , KeyActionState::released,
		new MoveCommand(ConnectionDirection::downLeft, m_pMover) });
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_q , KeyActionState::released,
		new MoveCommand(ConnectionDirection::upLeft, m_pMover) });
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_z , KeyActionState::released,
		new MoveCommand(ConnectionDirection::upRight, m_pMover) });
}

void CharacterController::Update()
{
}

void CharacterController::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterController).name());

	BehaviourComponent::Serialize(writer);
}