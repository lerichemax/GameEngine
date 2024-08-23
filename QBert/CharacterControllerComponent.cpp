#include "PCH.h"
#include "CharacterControllerComponent.h"

//void CharacterControllerComponent::Initialize()
//{
//	m_pMover = GetGameObject()->GetComponent<CharacterMovement>();
//}  
//  
//void CharacterControllerComponent::Start()
//{
//	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_d , KeyActionState::released,
//		new MoveCommand(ConnectionDirection::downRight) });
//	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_s , KeyActionState::released,
//		new MoveCommand(ConnectionDirection::downLeft) });
//	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_q , KeyActionState::released,
//		new MoveCommand(ConnectionDirection::upLeft) });
//	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_z , KeyActionState::released,
//		new MoveCommand(ConnectionDirection::upRight) });
//
//	FindComponentOfType<GameManagerBehaviour>()->OnGamePaused.Subscribe([this](bool isPaused) {
//		m_BlockInput = isPaused;
//		});
//}
//
//void CharacterControllerComponent::Update()
//{
//	if (!m_BlockInput)
//	{
//		InputManager::GetInstance().HandleInput(SDLK_d, GetGameObject());
//		InputManager::GetInstance().HandleInput(SDLK_s, GetGameObject());
//		InputManager::GetInstance().HandleInput(SDLK_q, GetGameObject());
//		InputManager::GetInstance().HandleInput(SDLK_z, GetGameObject());
//	}
//}

void CharacterControllerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterControllerComponent).name());

	Component::Serialize(writer);
}