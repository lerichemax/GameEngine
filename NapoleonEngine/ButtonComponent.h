#pragma once
#include "Component.h"
#include "Command.h"

#include "Factories.h"
#include "Event.h"

#include <memory>

template<typename T>
concept CommandDerived = std::derived_from<T, Command>;

class ButtonComponent : public ecs::Component
{
	friend class UiSystem;

public:
	ButtonComponent();
	~ButtonComponent() {};

	template<CommandDerived T> void SetOnClickFunction(T* func);
	template<CommandDerived T> void SetOnSelectFunction(T* func);
	template<CommandDerived T> void SetOnDeselectFunction(T* func);

	PROPERTY(glm::vec2, Dimensions);

	PROPERTY(bool, bVisualize);

	EventHandler<ButtonComponent> OnSelect;
	EventHandler<ButtonComponent> OnDeselect;
	EventHandler<ButtonComponent> OnClick;

	virtual void Serialize(StreamWriter& writer) const;
	virtual void Deserialize(JsonReader* const reader, SerializationMap& context);

	virtual void RestoreContext(JsonReader* const reader, SerializationMap const& context) override;

private:
	PROPERTY(std::unique_ptr<Command>, m_pOnClick);
	PROPERTY(std::unique_ptr<Command>, m_pOnSelect);
	PROPERTY(std::unique_ptr<Command>, m_pOnDeselect);

	bool m_bIsSelected;

	void Select();
	void Deselect();
	void Click();
};

SERIALIZE_CLASS(ButtonComponent)

template<CommandDerived T> 
void ButtonComponent::SetOnClickFunction(T* func) //pass command as ref ?
{
	Factory<Command>::Get().RegisterType<T>([]() {
		return new T{};
		});

	m_pOnClick.reset(func);
}

template<CommandDerived T> 
void ButtonComponent::SetOnSelectFunction(T* func)
{
	Factory<Command>::Get().RegisterType<T>([]() {
		return new T{};
		});
	m_pOnSelect.reset(func);
}

template<CommandDerived T> 
void ButtonComponent::SetOnDeselectFunction(T* func)
{
	Factory<Command>::Get().RegisterType<T>([]() {
		return new T{};
		});
	m_pOnDeselect.reset(func);
}