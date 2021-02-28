#pragma once
#include "SceneObject.h"

namespace dae
{
	class Texture2D;
	class GameObject : public SceneObject
	{
	public:
		GameObject(Tag tag);
		virtual ~GameObject();
		GameObject(const GameObject & other) = delete;
		GameObject(GameObject && other) = delete;
		GameObject& operator=(const GameObject & other) = delete;
		GameObject& operator=(GameObject && other) = delete;
		
		void Update(float deltaTime) override;
	};
	

}

