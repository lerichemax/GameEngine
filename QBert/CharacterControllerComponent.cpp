#include "PCH.h"
#include "CharacterControllerComponent.h"

//void CharacterControllerComponent::Initialize()
//{
//	m_pMover = GetGameObject()->GetComponent<CharacterMovement>();
//}  
//  
//void CharacterControllerComponent::Start()
//{
//	InputManager::Get().AddInputAction(new InputAction{ SDLK_d , KeyActionState::released,
//		new MoveCommand(ConnectionDirection::downRight) });
//	InputManager::Get().AddInputAction(new InputAction{ SDLK_s , KeyActionState::released,
//		new MoveCommand(ConnectionDirection::downLeft) });
//	InputManager::Get().AddInputAction(new InputAction{ SDLK_q , KeyActionState::released,
//		new MoveCommand(ConnectionDirection::upLeft) });
//	InputManager::Get().AddInputAction(new InputAction{ SDLK_z , KeyActionState::released,
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
//		InputManager::Get().HandleInput(SDLK_d, GetGameObject());
//		InputManager::Get().HandleInput(SDLK_s, GetGameObject());
//		InputManager::Get().HandleInput(SDLK_q, GetGameObject());
//		InputManager::Get().HandleInput(SDLK_z, GetGameObject());
//	}
//}

void CharacterControllerComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(CharacterControllerComponent)).hash_code()));

	Component::Serialize(writer);
}