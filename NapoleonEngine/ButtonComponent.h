#pragma once
#include "Component.h"
#include "Command.h"

#include "Event.h"

#include <memory>

template<typename T>
concept CommandDerived = std::derived_from<T, Command>;

class ButtonComponent : public ecs::Component
{
	friend class UiSystem;

public:
	ButtonComponent();

	template<CommandDerived T> void SetOnClickFunction(T* func);
	template<CommandDerived T> void SetOnSelectFunction(T* func);
	template<CommandDerived T> void SetOnDeselectFunction(T* func);

	PROPERTY(glm::vec2, Dimensions);

	PROPERTY(bool, bVisualize);

	EventHandler<ButtonComponent> OnSelect;
	EventHandler<ButtonComponent> OnDeselect;
	EventHandler<ButtonComponent> OnClick;

private:
	PROPERTY(Command*, m_pOnClick);
	PROPERTY(Command*, m_pOnSelect);
	PROPERTY(Command*, m_pOnDeselect);

	bool m_bIsSelected;

	void Select();
	void Deselect();
	void Click();
};

SERIALIZE_CLASS(ButtonComponent, ecs::Component)

template<CommandDerived T> 
void ButtonComponent::SetOnClickFunction(T* func) //pass command as ref ?
{
	m_pOnClick = func;
}

template<CommandDerived T> 
void ButtonComponent::SetOnSelectFunction(T* func)
{
	m_pOnSelect = func;
}

template<CommandDerived T> 
void ButtonComponent::SetOnDeselectFunction(T* func)
{
	m_pOnDeselect = func;
}