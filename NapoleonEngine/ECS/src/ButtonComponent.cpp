#include "PCH.h"
#include "ButtonComponent.h"

#include "Command.h"
#include "InputManager.h"
#include "GameObject.h"
#include "TransformComponent.h"

ButtonComponent::ButtonComponent()
	:Component(true),
	Dimensions(100, 50), //by default
	m_bIsSelected(false),
	bVisualize(false),
	m_pOnClick(nullptr),
	m_pOnSelect(nullptr),
	m_pOnDeselect(nullptr)
{
}

ButtonComponent::~ButtonComponent()
{
	SAFE_DELETE(m_pOnClick);
	SAFE_DELETE(m_pOnSelect);
	SAFE_DELETE(m_pOnDeselect);
}

void ButtonComponent::Select()
{
	m_bIsSelected = true;
	OnSelect.Notify();
}

void ButtonComponent::Deselect()
{
	m_bIsSelected = false;
	OnDeselect.Notify();
}

void ButtonComponent::Click()
{
	OnClick.Notify();
}
