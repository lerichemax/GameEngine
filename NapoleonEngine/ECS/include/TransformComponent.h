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
class TransformComponent final: public ecs::Component
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

	glm::vec2 GetLocation() const;
	glm::vec2 GetLocalLocation() const;
	glm::vec2 GetScale() const;
	glm::vec2 GetLocalScale() const;
	float GetRotation() const;
	float GetLocalRotation() const;

	glm::mat3x3 const& GetWorldTransformMatrix() const;

	void SetParent(TransformComponent* const pParent);

	static glm::mat3x3 BuildTransformMatrix(glm::vec2 const& translation, glm::vec2 const& scale, float rotation);

private:

	PROPERTY(glm::mat3x3, m_WorldTransformMatrix);
	PROPERTY(glm::mat3x3, m_LocalTransformMatrix);

	PROPERTY(TransformComponent*, m_pParent);
};

SERIALIZE_CLASS(TransformComponent, ecs::Component) // special case. Maybe exclude from reflection ? At least for properties 
