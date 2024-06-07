#pragma once
#include "Component.h"

#include <memory>

class Command;
class ButtonComponent final : public Component
{
public:
	ButtonComponent(float width, float height);
		
	ButtonComponent(ButtonComponent&& other) = delete;
	ButtonComponent& operator=(ButtonComponent const& rhs) = delete;
	ButtonComponent& operator=(ButtonComponent&& rhs) = delete;
	~ButtonComponent();
		
	void SetWidth(float width) { m_Dimensions.x = width; }
	void SetHeight(float height) { m_Dimensions.y = height; }
	void SetDimensions(glm::vec2 const& dim) { m_Dimensions = dim; }

	void SetIsHighlighted(bool isSelected) { m_IsSelected = isSelected; }
		
	void SetOnClickFunction(Command* func);
	void SetOnSelectFunction(Command* func);
	void SetOnDeselectFunction(Command* func);
		
	void SetVisualize(bool visualize) { m_bVisualize = visualize; }

protected:
	void Initialize() override {};
	void Update() override;
	ButtonComponent* Clone() const override { return new ButtonComponent(*this); }
	
private:
	Command* m_pOnClick;
	Command* m_pOnSelect;
	Command* m_pOnDeselect;
		
	glm::vec2 m_Dimensions;
		
	bool m_IsSelected;
	bool m_bVisualize;

		
	ButtonComponent(ButtonComponent const& other);
};

struct ECS_ButtonComponent : public ECS_Component
{
	friend class UiSystem;

public:
	ECS_ButtonComponent(float width, float height);
	ECS_ButtonComponent() = default;

	void SetOnClickFunction(Command* func);
	void SetOnSelectFunction(Command* func);
	void SetOnDeselectFunction(Command* func);

	glm::vec2 m_Dimensions;

	bool m_bVisualize;

	virtual void Serialize(StreamWriter& writer) const override;
	virtual void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	virtual void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

private:
	std::shared_ptr<Command> m_pOnClick = nullptr; 
	std::shared_ptr<Command> m_pOnSelect = nullptr; 
	std::shared_ptr<Command> m_pOnDeselect= nullptr; 

	bool m_IsSelected;
};
