#pragma once
#include "GameObject.h"

namespace empire
{
	class UIObject final : public GameObject
	{
	public:
		UIObject();
		~UIObject() = default;
		void Update(float deltaTime) override;
	
	private:
	};
}


