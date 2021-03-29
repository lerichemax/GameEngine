#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)

#include "Component.h"

namespace empire
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
		void Translate(float x, float y, float z = 0.f);
	private:
		glm::vec3 m_Position;
	};
}
