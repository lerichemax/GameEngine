#pragma once
#include "GameObject.h"
#include "TextRendererComponent.h"

namespace empire
{
	class UIObject final : public GameObject
	{
	public:
		UIObject();
		~UIObject() = default;
		void Update() override;
	
	private:
	};
}


