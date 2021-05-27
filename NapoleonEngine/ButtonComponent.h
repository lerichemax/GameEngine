#pragma once
#include "Component.h"

#include <glm/glm.hpp>
#include <functional>
namespace empire
{
	class ButtonComponent final : public Component
	{
	public:
		typedef std::function<void(GameObject* pBtnObject)> ButtonAction;
		
		ButtonComponent(float width, float height);
		ButtonComponent* Clone() { return new ButtonComponent(*this); }

		void Update() override;

		void SetWidth(float width) { m_Dimensions.x = width; }
		void SetHeight(float height) { m_Dimensions.y = height; }
		void SetDimensions(glm::vec2 const& dim) { m_Dimensions = dim; }

		void SetIsHighlighted(bool isSelected) { m_IsSelected = isSelected; }
		
		void SetOnClickFunction(ButtonAction func) { m_OnClick = func; }
		void SetOnSelectFunction(ButtonAction func) { m_OnSelect = func; }
		void SetOnDeselectFunction(ButtonAction func) { m_OnDeselect = func; }
		
		void SetVisualize(bool visualize) { m_bVisualize = visualize; }
	
	private:
		ButtonAction m_OnClick;
		ButtonAction m_OnSelect;
		ButtonAction m_OnDeselect;
		
		glm::vec2 m_Dimensions;
		
		bool m_IsSelected;
		bool m_bVisualize;

		void Initialize() override {};
	};
}