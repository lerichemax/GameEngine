#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#pragma warning(pop)

#include "Component.h"
#include "System.h"
#include "Scene.h"

struct lua_State;
class TransformComponent final : public ecs::Component
{
	friend class TransformSystem;
	friend class GameObject;
	friend class Renderer;
	friend class Scene;

public:
	TransformComponent();

	void SetLocalLocation(vec2 const& loc);
	void SetLocalLocation(float x, float y);
	void SetLocation(vec2 const& translation);
	void SetLocation(float x, float y);
	void Scale(vec2 const& scale);
	void Scale(float scale);
	void Rotate(float rotation);

	glm::vec2 GetLocation();
	glm::vec2 GetLocalLocation();
	glm::vec2 GetScale();
	glm::vec2 GetLocalScale();
	float GetRotation();
	float GetLocalRotation();

	glm::mat3x3 const& GetWorldTransformMatrix() ;

	static glm::mat3x3 BuildTransformMatrix(glm::vec2 const& translation, glm::vec2 const& scale, float rotation);

	PROPERTY(TransformComponent*, m_pParent);

private:
	PROPERTY(glm::mat3x3, m_WorldTransformMatrix);
	PROPERTY(glm::mat3x3, m_LocalTransformMatrix);

	bool m_bWasChanged;

	bool WasChanged() const;
	void UpdateTransformHierarchy();
};

SERIALIZE_CLASS(TransformComponent, ecs::Component)
