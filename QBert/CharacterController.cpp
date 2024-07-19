#include "PCH.h"
#include "CharacterController.h"

#include "QBert.h"

#include "InputManager.h"

void CharacterController::Initialize()
{
	m_pQbert = GetGameObject()->GetComponent<QBert>();
}
void CharacterController::Update()
{
	if (InputManager::GetInstance().IsUp(SDLK_x))
	{
		printf("bonjour \n");
	}
	
}

void CharacterController::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(CharacterController).name());

	BehaviourComponent::Serialize(writer);
}