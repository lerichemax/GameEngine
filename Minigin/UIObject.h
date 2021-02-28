#pragma once
#include "SceneObject.h"

namespace dae
{
	class UIObject final : public SceneObject
	{
	public:
		UIObject();
		~UIObject() = default;
		void Update(float deltaTime) override;
	
	private:
	};
}


