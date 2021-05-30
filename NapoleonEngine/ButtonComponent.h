#pragma once
#include "Component.h"

#include <glm/glm.hpp>
#include <functional>
namespace empire
{
	class Command;
	class ButtonComponent final : public Component
	{
	public:
		
		ButtonComponent(float width, float height);
		ButtonComponent* Clone() { return new ButtonComponent(*this); }
		ButtonComponent(ButtonComponent&& other) = delete;
		ButtonComponent& operator=(ButtonComponent const& rhs) = delete;
		ButtonComponent& operator=(ButtonComponent&& rhs) = delete;
		~ButtonComponent();
		
		void Update() override;

		void SetWidth(float width) { m_Dimensions.x = width; }
		void SetHeight(float height) { m_Dimensions.y = height; }
		void SetDimensions(glm::vec2 const& dim) { m_Dimensions = dim; }

		void SetIsHighlighted(bool isSelected) { m_IsSelected = isSelected; }
		
		void SetOnClickFunction(Command* func) { m_pOnClick = func; }
		void SetOnSelectFunction(Command* func) { m_pOnSelect = func; }
		void SetOnDeselectFunction(Command* func) { m_pOnDeselect = func; }
		
		void SetVisualize(bool visualize) { m_bVisualize = visualize; }
	
	private:
		Command* m_pOnClick;
		Command* m_pOnSelect;
		Command* m_pOnDeselect;
		
		glm::vec2 m_Dimensions;
		
		bool m_IsSelected;
		bool m_bVisualize;

		void Initialize() override {};
		ButtonComponent(ButtonComponent const& other);
	};
}