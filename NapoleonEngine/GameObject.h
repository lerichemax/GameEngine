#pragma once
#include "SceneObject.h"

namespace empire
{
	class Texture2D;
	class GameObject final : public SceneObject
	{
	public:
		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject & other) = delete;
		GameObject(GameObject && other) = delete;
		GameObject& operator=(const GameObject & other) = delete;
		GameObject& operator=(GameObject && other) = delete;
		
		void Update(float deltaTime) override;
	};
	

}

