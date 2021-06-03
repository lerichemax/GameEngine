#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#pragma warning(pop)

#include "Component.h"

namespace empire
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent();
		TransformComponent(float x, float y);
		TransformComponent* Clone() override { return new TransformComponent(*this); };
		TransformComponent(TransformComponent const& other) = default;
		TransformComponent(TransformComponent&& other) = default;
		TransformComponent& operator=(TransformComponent const& rhs);
		TransformComponent& operator=(TransformComponent&& rhs) = default;
		~TransformComponent() = default;
		
		void Update() override;
		
		const glm::vec2& GetPosition() const { return m_Position; }
		const glm::vec2& GetWorldPosition() const { return m_WorldPosition; }
		const glm::vec2& GetScale() const { return m_Scale; }
		const glm::vec2& GetWorldScale() const { return m_WorldScale; }
		float GetRotation() const { return m_Rotation; }
		float GetWorldRotation() const { return m_WorldRotation; }
		
		void Translate(float x, float y);
		void Translate(glm::vec2 const& pos);
		void Scale(float x, float y);
		void Scale(glm::vec2 const& scale);
		void Scale(float uniformScale);
		void Rotate(float rot);

		void SetWorldPosition(glm::vec2 const& worldPos);
		void SetWorldPosition(float x, float y);
		
		glm::mat3x3 GetWorldMatrix() const { return m_World; }
		glm::mat3x3 GetWorldToLocalMatrix() const {return glm::inverse(m_World); }
	
	private:
		glm::vec2 m_Position;
		glm::vec2 m_Scale;
		float m_Rotation;

		glm::mat3x3 m_World;
		glm::vec2 m_WorldPosition;
		glm::vec2 m_WorldScale;
		float m_WorldRotation;
		
		void Initialize() override;
	};
}
