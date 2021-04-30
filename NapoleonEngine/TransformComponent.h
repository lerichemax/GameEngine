#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "Component.h"

namespace empire
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent();
		TransformComponent(float x, float y, float z = 0.0f);
		TransformComponent* Clone() override { return new TransformComponent(*this); };
		TransformComponent(TransformComponent const& other) = default;
		TransformComponent(TransformComponent&& other) = default;
		TransformComponent& operator=(TransformComponent const& rhs);
		TransformComponent& operator=(TransformComponent&& rhs) = default;
		~TransformComponent() = default;
		
		void Update() override {}
		
		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetScale() const { return m_Scale; }
		float GetRotation() const { return m_Rotation; }
		
		void Translate(float x, float y, float z = 0.f);
		void Translate(glm::vec3 const& pos);
		void Translate(glm::vec2 const& pos);
		void Scale(float x, float y, float z = 1);
		void Scale(glm::vec3 const& scale);
		void Scale(float uniformScale);
		void Rotate(float rot);
	
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		float m_Rotation;
		
		void Initialize() override {}
	};
}
