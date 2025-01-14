#include "PCH.h"
#include "UiSystem.h"
#include "Registry.h"
#include "InputManager.h"
#include "Command.h"

#include "ButtonComponent.h"
#include "TransformComponent.h"

void UiSystem::Update()
{
	auto view = m_pRegistry->GetView<TransformComponent, ButtonComponent>();

	for (Entity entity : view)
	{
		auto btn = m_pRegistry->GetComponent<ButtonComponent>(entity);

		if (!btn->IsActive())
		{
			continue;
		}

		auto transform = m_pRegistry->GetComponent<TransformComponent>(entity);

		if (btn->bVisualize)
		{
			Debugger::Get().DrawDebugRectangle(transform->GetLocation(),
				static_cast<int>(btn->Dimensions.x), static_cast<int>(btn->Dimensions.y),
				Color{ 255,0,0,1 });
		}
		glm::vec2 const mousePos = InputManager::Get().GetMousePosition();
		glm::vec2 const Pos = transform->GetLocation();

		GameObject btnObject{ btn->GetEntity(), m_pRegistry };

		if (mousePos.x >= Pos.x && mousePos.x <= Pos.x + btn->Dimensions.x &&
			mousePos.y >= Pos.y && mousePos.y <= Pos.y + btn->Dimensions.y)
		{
			if (!btn->m_bIsSelected && btn->m_pOnSelect)
			{
				btn->m_pOnSelect->Execute(btnObject);
			}
			btn->Select();
		}
		else
		{
			if (btn->m_bIsSelected && btn->m_pOnDeselect)
			{
				btn->m_pOnDeselect->Execute(btnObject);
			}
			btn->Deselect();
		}

		if (InputManager::Get().IsLMBPressed())
		{
			if (btn->m_bIsSelected && btn->m_pOnClick)
			{
				btn->m_pOnClick->Execute(btnObject);
			}

			btn->Click();
		}
	}
}