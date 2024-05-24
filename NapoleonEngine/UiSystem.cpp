#include "PCH.h"
#include "UiSystem.h"
#include "Coordinator.h"
#include "InputManager.h"
#include "Command.h"

#include "ButtonComponent.h"
#include "TransformComponent.h"


UiSystem::UiSystem(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());
	signature.set(pRegistry->GetComponentType<ECS_ButtonComponent>());

	pRegistry->SetSystemSignature<UiSystem>(signature);
}

void UiSystem::Update(ComponentManager* const pComponentManager)
{
	for (Entity const& entity : m_Entities)
	{
		auto btn = pComponentManager->GetComponent<ECS_ButtonComponent>(entity);

		if (!btn->IsActive())
		{
			return;
		}

		auto transform = pComponentManager->GetComponent<ECS_TransformComponent>(entity);

		if (btn->m_bVisualize)
		{
			Debugger::GetInstance().DrawDebugRectangle(transform->GetPosition(),
				static_cast<unsigned int>(btn->m_Dimensions.x), static_cast<unsigned int>(btn->m_Dimensions.y),
				Color{ 255,0,0,1 });
		}
		glm::vec2 const mousePos = InputManager::GetInstance().GetMousePosition();
		glm::vec2 const pos = transform->GetPosition();

		if (mousePos.x >= pos.x && mousePos.x <= pos.x + btn->m_Dimensions.x &&
			mousePos.y >= pos.y && mousePos.y <= pos.y + btn->m_Dimensions.y)
		{
			if (!btn->m_IsSelected && btn->m_pOnSelect)
			{
				btn->m_pOnSelect->Execute();
			}

			btn->m_IsSelected = true;
		}
		else
		{
			if (btn->m_IsSelected && btn->m_pOnDeselect)
			{
				btn->m_pOnDeselect->Execute();
			}
			btn->m_IsSelected = false;
		}

		if (InputManager::GetInstance().IsLMBPressed() && btn->m_IsSelected && btn->m_pOnClick)
		{
			btn->m_pOnClick->Execute();
		}
	}
}