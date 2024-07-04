#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include "SceneManager.h"
#include "Scene.h"

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

inline glm::vec2 TransformPoint(glm::vec2 const& point, glm::mat3x3 const& mat)
{
	glm::vec3 toTransform{ point.x, point.y, 1 };
	return mat * toTransform;
}

inline glm::vec2 TransformVector(glm::vec2 const& vec, glm::mat3x3 const& mat)
{
	glm::vec3 toTransform{ vec.x, vec.y, 0 };
	return mat * toTransform;
}

inline std::shared_ptr<GameObject> Instantiate(std::string const& name)
{
	return SceneManager::GetInstance().GetActiveScene()->InstantiatePrefab(name);
}

template <ComponentDerived T>
inline std::shared_ptr<T> FindObjectOfType()
{
	return nullptr;
}