#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#pragma warning(pop)

#include "Component.h"

namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent() = default;
		TransformComponent(float x, float y, float z = 0.0f);
		TransformComponent(TransformComponent const& other) = default;
		TransformComponent(TransformComponent&& other) = default;
		TransformComponent& operator=(TransformComponent const& rhs);
		TransformComponent& operator=(TransformComponent&& rhs) = default;
		~TransformComponent() = default;

		void Update() override {}
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_Position;
	};
}
