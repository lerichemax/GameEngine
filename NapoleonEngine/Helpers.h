#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Camera.h"

#define SAFE_DELETE(ptr) \
	if (ptr != nullptr) \
	{\
		delete(ptr);\
		ptr = nullptr;\
	}\

#define IS_VALID(ptr) (ptr != nullptr) \

#define NULL_ENTITY -1


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