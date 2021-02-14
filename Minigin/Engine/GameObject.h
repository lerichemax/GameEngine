#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Component.h"
#include "RendererComponent.h"

namespace dae
{
	class Texture2D;
	class GameObject : public SceneObject
	{
	public:
		GameObject() = default;
		GameObject(RendererComponent* pRenderComp);
		virtual ~GameObject();
		GameObject(const GameObject & other) = delete;
		GameObject(GameObject && other) = delete;
		GameObject& operator=(const GameObject & other) = delete;
		GameObject& operator=(GameObject && other) = delete;
		
		void Update() override;

		void SetTexture(const std::string& filename);


	
	private:
	};
	

}

