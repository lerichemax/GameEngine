#include "PCH.h"
#include "ButtonComponent.h"

#include "InputManager.h"
#include "GameObject.h"

empire::ButtonComponent::ButtonComponent(float width, float height)
	:Component(),
	m_Dimensions(width, height),
	m_IsSelected(false),
	m_bVisualize(false)
{
	
}

void empire::ButtonComponent::Update()
{
	if (m_bVisualize)
	{
		Debugger::GetInstance().DrawDebugRectangle(m_pGameObject->GetTransform()->GetPosition(), 
			(unsigned int)m_Dimensions.x, (unsigned int)m_Dimensions.y, Color{1,1,1,1});
	}
	glm::vec2 mousePos = InputManager::GetInstance().GetMousePosition();
	glm::vec2 pos = m_pGameObject->GetTransform()->GetPosition();
	
	if (mousePos.x >= pos.x && mousePos.x <= pos.x + m_Dimensions.x &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + m_Dimensions.y)
	{
		if (!m_IsSelected && m_OnSelect)
		{
			m_OnSelect(m_pGameObject);
		}

		m_IsSelected = true;
	}
	else
	{
		if (m_IsSelected && m_OnDeselect)
		{
			m_OnDeselect(m_pGameObject);
		}
		m_IsSelected = false;
	}

	if (InputManager::GetInstance().IsMousePressed() && m_IsSelected && m_OnClick)
	{
		m_OnClick(m_pGameObject);
	}
}