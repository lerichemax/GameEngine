#include "PCH.h"
#include "UiSystem.h"
#include "Coordinator.h"
#include "InputManager.h"
#include "Command.h"

#include "ButtonComponent.h"
#include "TransformComponent.h"

void UiSystem::Update()
{
	for (Entity const& entity : m_Entities)
	{
		auto btn = m_pCompManager->GetComponent<ButtonComponent>(entity);

		if (!btn->IsActive())
		{
			continue;
		}

		auto transform = m_pCompManager->GetComponent<ECS_TransformComponent>(entity);

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
				btn->m_pOnSelect->Execute(btn->GetGameObject());
			}
			btn->OnSelect.Notify();

			btn->m_IsSelected = true;
		}
		else
		{
			if (btn->m_IsSelected && btn->m_pOnDeselect)
			{
				btn->m_pOnDeselect->Execute(btn->GetGameObject());
			}
			btn->OnDeselect.Notify();

			btn->m_IsSelected = false;
		}

		if (InputManager::GetInstance().IsLMBPressed())
		{
			if (btn->m_IsSelected && btn->m_pOnClick)
			{
				btn->m_pOnClick->Execute(btn->GetGameObject());
			}

			btn->OnClick.Notify();
		}
	}
}

void UiSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());
	signature.set(pRegistry->GetComponentType<ButtonComponent>());

	pRegistry->SetSystemSignature<UiSystem>(signature);
}