#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

inline glm::mat3x3 BuildTransformMatrix(glm::vec2 const& pos, float rot, glm::vec2 const& scale)
{
	auto identityMatrix = glm::mat3(1.0f);

	glm::mat3x3 trans{};
	glm::mat3x3 rotation{};
	glm::mat3x3 scaling{};

	trans = glm::translate(identityMatrix, pos);

#ifdef GLM_FORCE_RADIANS
	rot = glm::rotate(identityMatrix, glm::radians(m_WorldRotation));
#else
	rotation = glm::rotate(identityMatrix, rot);
#endif

	scaling = glm::scale(identityMatrix, scale);

	return  trans * rotation * scaling;
}

template <typename T>
inline void SafeDelete(T& pToDelete)
{
	if (pToDelete != nullptr)
	{
		delete(pToDelete);
		pToDelete = nullptr;
	}
}