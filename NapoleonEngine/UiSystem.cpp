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
		auto btn = m_pRegistry->GetComponent<ButtonComponent>(entity);

		if (!btn->IsActive())
		{
			continue;
		}

		auto transform = m_pRegistry->GetComponent<TransformComponent>(entity);

		if (btn->m_bVisualize)
		{
			Debugger::Get().DrawDebugRectangle(transform->GetLocation(),
				static_cast<unsigned int>(btn->m_Dimensions.x), static_cast<unsigned int>(btn->m_Dimensions.y),
				Color{ 255,0,0,1 });
		}
		glm::vec2 const mousePos = InputManager::Get().GetMousePosition();
		glm::vec2 const Pos = transform->GetLocation();

		if (mousePos.x >= Pos.x && mousePos.x <= Pos.x + btn->m_Dimensions.x &&
			mousePos.y >= Pos.y && mousePos.y <= Pos.y + btn->m_Dimensions.y)
		{
			if (!btn->m_bIsSelected && btn->m_pOnSelect)
			{
				btn->m_pOnSelect->Execute(btn->GetGameObject());
			}
			btn->Select();
		}
		else
		{
			if (btn->m_bIsSelected && btn->m_pOnDeselect)
			{
				btn->m_pOnDeselect->Execute(btn->GetGameObject());
			}
			btn->Deselect();
		}

		if (InputManager::Get().IsLMBPressed())
		{
			if (btn->m_bIsSelected && btn->m_pOnClick)
			{
				btn->m_pOnClick->Execute(btn->GetGameObject());
			}

			btn->Click();
		}
	}
}

void UiSystem::SetSignature() const
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());
	signature.set(m_pRegistry->GetComponentType<ButtonComponent>());

	m_pRegistry->SetSystemSignature<UiSystem>(signature);
}