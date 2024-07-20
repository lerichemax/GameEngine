#include "PCH.h"
#include "CharacterController.h"

#include "QBert.h"

#include "MoveCommand.h"

#include "InputManager.h"

void CharacterController::Initialize()
{
	m_pQbert = GetGameObject()->GetComponent<QBert>();
}  
  
void CharacterController::Start()
{
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_d , KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downRight, m_pQbert) });
	InputManager::GetInstance().AddInputAction(new InputAction{ SDLK_s , KeyActionState::pressed,
		new MoveCommand(ConnectionDirection::downLeft, m_pQbert) });
}

void CharacterController::Update()
{

}

void CharacterController::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterController).name());

	BehaviourComponent::Serialize(writer);
}