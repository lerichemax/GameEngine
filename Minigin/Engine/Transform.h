#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#pragma warning(pop)

namespace dae
{
	class Transform final
	{
	public:
		Transform() = default;
		Transform& operator=(Transform const& rhs);
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_Position;
	};
}
