#include "PCH.h"
#include "CharacterController.h"
#include "CharacterMovement.h"
#include "InputManager.h"

#include "MoveCommand.h"
#include "GameManager.h"

void CharacterController::Initialize()
{
	m_pMover = GetGameObject()->GetComponent<CharacterMovement>();
}  
  
void CharacterController::Start()
{
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_d , KeyActionState::released,
		new MoveCommand(ConnectionDirection::downRight) });
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_s , KeyActionState::released,
		new MoveCommand(ConnectionDirection::downLeft) });
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_q , KeyActionState::released,
		new MoveCommand(ConnectionDirection::upLeft) });
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_z , KeyActionState::released,
		new MoveCommand(ConnectionDirection::upRight) });

	FindComponentOfType<GameManagerBehaviour>()->OnGamePaused.Subscribe([this](bool isPaused) {
		m_BlockInput = isPaused;
		});
}

void CharacterController::Update()
{
	if (!m_BlockInput)
	{
		InputManager::GetInstance().HandleInput(SDLK_d, GetGameObject());
		InputManager::GetInstance().HandleInput(SDLK_s, GetGameObject());
		InputManager::GetInstance().HandleInput(SDLK_q, GetGameObject());
		InputManager::GetInstance().HandleInput(SDLK_z, GetGameObject());
	}
}

void CharacterController::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterController).name());

	BehaviourComponent::Serialize(writer);
}