#include "PCH.h"
#include "ButtonComponent.h"


#include "Command.h"
#include "InputManager.h"
#include "GameObject.h"
#include "TransformComponent.h"

empire::ButtonComponent::ButtonComponent(float width, float height)
	:Component(),
	m_Dimensions(width, height),
	m_IsSelected(false),
	m_bVisualize(false),
	m_pOnClick(nullptr),
	m_pOnSelect(nullptr),
	m_pOnDeselect(nullptr)
{
}

empire::ButtonComponent::ButtonComponent(empire::ButtonComponent const& other)
	:m_Dimensions(other.m_Dimensions),
	m_IsSelected(other.m_IsSelected),
	m_bVisualize(other.m_bVisualize),
	m_pOnClick(other.m_pOnClick != nullptr ? other.m_pOnClick->Clone() : nullptr),
	m_pOnSelect(other.m_pOnSelect != nullptr ? other.m_pOnSelect->Clone() : nullptr),
	m_pOnDeselect(other.m_pOnDeselect != nullptr ? other.m_pOnDeselect->Clone() : nullptr)
{}

empire::ButtonComponent::~ButtonComponent()
{
	SafeDelete(m_pOnClick);
	SafeDelete(m_pOnSelect);
	SafeDelete(m_pOnDeselect);
}

void empire::ButtonComponent::Update()
{
	if (m_bVisualize)
	{
		Debugger::GetInstance().DrawDebugRectangle(m_pGameObject->GetTransform()->GetPosition(), 
			static_cast<unsigned int>(m_Dimensions.x), static_cast<unsigned int>(m_Dimensions.y),
			Color{255,0,0,1});
	}
	glm::vec2 const mousePos = InputManager::GetInstance().GetMousePosition();
	glm::vec2 const pos = m_pGameObject->GetTransform()->GetPosition();
	
	if (mousePos.x >= pos.x && mousePos.x <= pos.x + m_Dimensions.x &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + m_Dimensions.y)
	{
		if (!m_IsSelected && m_pOnSelect)
		{
			m_pOnSelect->Execute();
		}

		m_IsSelected = true;
	}
	else
	{
		if (m_IsSelected && m_pOnDeselect)
		{
			m_pOnDeselect->Execute();
		}
		m_IsSelected = false;
	}

	if (InputManager::GetInstance().IsLMBPressed() && m_IsSelected && m_pOnClick)
	{
		m_pOnClick->Execute();
	}

}


void ButtonComponent::SetOnClickFunction(Command* func)
{
	m_pOnClick = func;
}

void ButtonComponent::SetOnSelectFunction(Command* func)
{
	m_pOnSelect = func;
}
void ButtonComponent::SetOnDeselectFunction(Command* func)
{
	m_pOnDeselect = func;
}