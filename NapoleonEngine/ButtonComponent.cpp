#include "PCH.h"
#include "ButtonComponent.h"

#include "Command.h"
#include "InputManager.h"
#include "GameObject.h"
#include "TransformComponent.h"

ButtonComponent::ButtonComponent(float width, float height)
	:Component(),
	m_Dimensions(width, height),
	m_IsSelected(false),
	m_bVisualize(false),
	m_pOnClick(nullptr),
	m_pOnSelect(nullptr),
	m_pOnDeselect(nullptr)
{
}

ButtonComponent::ButtonComponent(ButtonComponent const& other)
	:m_Dimensions(other.m_Dimensions),
	m_IsSelected(other.m_IsSelected),
	m_bVisualize(other.m_bVisualize),
	m_pOnClick(other.m_pOnClick != nullptr ? other.m_pOnClick->Clone() : nullptr),
	m_pOnSelect(other.m_pOnSelect != nullptr ? other.m_pOnSelect->Clone() : nullptr),
	m_pOnDeselect(other.m_pOnDeselect != nullptr ? other.m_pOnDeselect->Clone() : nullptr)
{}

ButtonComponent::~ButtonComponent()
{
	SafeDelete(m_pOnClick);
	SafeDelete(m_pOnSelect);
	SafeDelete(m_pOnDeselect);
}

void ButtonComponent::Update()
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

ECS_ButtonComponent::ECS_ButtonComponent(float width, float height)
	:ECS_Component(true),
	m_Dimensions(width, height),
	m_IsSelected(false),
	m_bVisualize(false),
	m_pOnClick(nullptr),
	m_pOnSelect(nullptr),
	m_pOnDeselect(nullptr)
{
}

ECS_ButtonComponent::ECS_ButtonComponent(ECS_ButtonComponent const& other)
	:ECS_Component(other),
	m_Dimensions(other.m_Dimensions),
	m_pOnClick(other.m_pOnClick != nullptr ? other.m_pOnClick->Clone() : nullptr),
	m_pOnSelect(other.m_pOnSelect != nullptr ? other.m_pOnSelect->Clone() : nullptr),
	m_pOnDeselect(other.m_pOnDeselect != nullptr ? other.m_pOnDeselect->Clone() : nullptr),
	m_IsSelected(other.m_IsSelected),
	m_bVisualize(other.m_bVisualize)
{

}

ECS_ButtonComponent::~ECS_ButtonComponent()
{
	SafeDelete(m_pOnSelect);
	SafeDelete(m_pOnDeselect);
	SafeDelete(m_pOnClick);
}

void ECS_ButtonComponent::SetOnClickFunction(Command* const func)
{
	delete m_pOnClick;
	m_pOnClick = func;
}

void ECS_ButtonComponent::SetOnSelectFunction(Command* const func)
{
	delete m_pOnSelect;
	m_pOnSelect = func;
}
void ECS_ButtonComponent::SetOnDeselectFunction(Command* const func)
{
	delete m_pOnDeselect;
	m_pOnDeselect = func;
}