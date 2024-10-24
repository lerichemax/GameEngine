#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"

#define SAFE_DELETE(ptr) \
	if (ptr != nullptr) \
	{\
		delete(ptr);\
		ptr = nullptr;\
	}\

#define IS_VALID(ptr) (ptr != nullptr) \

#define NULL_ENTITY -1


inline glm::mat3x3 BuildTransformMatrix(glm::vec2 const& Pos, float rot, glm::vec2 const& scale) //make part of transform
{
	auto identityMatrix = glm::mat3(1.0f);

	glm::mat3x3 trans{};
	glm::mat3x3 rotation{};
	glm::mat3x3 scaling{};

	trans = glm::translate(identityMatrix, Pos);

#ifdef GLM_FORCE_RADIANS
	rot = glm::rotate(identityMatrix, glm::radians(m_WorldRotation));
#else
	rotation = glm::rotate(identityMatrix, rot);
#endif

	scaling = glm::scale(identityMatrix, scale);

	return  trans * rotation * scaling;
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
	return SceneManager::Get().GetActiveScene()->InstantiatePrefab(name);
}

inline std::shared_ptr<GameObject> Instantiate(std::string const& name, glm::vec2 const& location)
{
	return SceneManager::Get().GetActiveScene()->InstantiatePrefab(name, location);
}

template <ComponentDerived T>
inline T* const FindComponentOfType()
{
	return SceneManager::Get().GetActiveScene()->m_pRegistry->FindComponentOfType<T>();
}

template <ComponentDerived T>
inline T* const FindComponentsOfType()
{
	return SceneManager::Get().GetActiveScene()->m_pRegistry->FindComponentsOfType<T>();
}
