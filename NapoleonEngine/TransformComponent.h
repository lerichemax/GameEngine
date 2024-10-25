#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#pragma warning(pop)

#include "Component.h"
#include "System.h"
#include "Scene.h"


class TransformComponent final: public Component
{
	friend class TransformSystem;
	friend class GameObject;
	friend class Renderer;
	friend class Scene;

public:
	TransformComponent();

	void SetLocalLocation(vec2 const& loc);
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

	void SetParent(TransformComponent* const pParent);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

private:

	glm::mat3x3 m_WorldTransformMatrix{};
	glm::mat3x3 m_LocalTransformMatrix{};


	TransformComponent* m_pParent{};

	glm::mat3x3 BuildTransformMatrix(glm::vec2 const& translation, glm::vec2 const& scale, float rotation);
};
