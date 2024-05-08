#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#pragma warning(pop)

#include "Component.h"
#include "System.h"

class TransformComponent final : public Component
{
public:
	TransformComponent();
	TransformComponent(float x, float y);
		
	TransformComponent(TransformComponent&& other) = delete;
	TransformComponent& operator=(TransformComponent const& rhs);
	TransformComponent& operator=(TransformComponent&& rhs) = delete;
	~TransformComponent() = default;

	const glm::vec2& GetPosition() const { return m_Position; }
	const glm::vec2& GetWorldPosition() const { return m_WorldPosition; }
	const glm::vec2& GetScale() const { return m_Scale; }
	const glm::vec2& GetWorldScale() const { return m_WorldScale; }
	float GetRotation() const { return m_Rotation; }
	float GetWorldRotation() const { return m_WorldRotation; }
		
	void Translate(float x, float y);
	void Translate(glm::vec2 const& pos);
	void Scale(float x, float y);
	void Scale(glm::vec2 const& scale);
	void Scale(float uniformScale);
	void Rotate(float rot);

	void SetWorldPosition(glm::vec2 const& worldPos);
	void SetWorldPosition(float x, float y);
		
	glm::mat3x3 GetWorldMatrix() const { return m_World; }
	glm::mat3x3 GetWorldToLocalMatrix() const {return glm::inverse(m_World); }
	
private:
	glm::vec2 m_Position;
	glm::vec2 m_Scale;
	float m_Rotation;

	glm::mat3x3 m_World;
	glm::vec2 m_WorldPosition;
	glm::vec2 m_WorldScale;
	float m_WorldRotation;
		
	void Initialize() override;
	void Update() override;
	TransformComponent* Clone() const override { return new TransformComponent(*this); };
	TransformComponent(TransformComponent const& other) = default;
};

struct ECS_TransformComponent : public ECS_Component
{
	friend class TransformSystem;

	ECS_TransformComponent() : ECS_Component(true) {}

	void Translate(vec2 const& translation);
	void Translate(float x, float y);
	void Scale(vec2 const& scale);
	void Scale(float scale);
	void Rotate(float rotation);

	const glm::vec2& GetPosition() const { return m_Position; }
	const glm::vec2& GetWorldPosition() const { return m_WorldPosition; }
	const glm::vec2& GetScale() const { return m_Scale; }
	const glm::vec2& GetWorldScale() const { return m_WorldScale; }
	float GetRotation() const { return m_Rotation; }
	float GetWorldRotation() const { return m_WorldRotation; }

	glm::vec2 m_Position{};
	glm::vec2 m_Scale{1.f, 1.f};
	float m_Rotation{};

private:
	glm::mat3x3 m_World{};
	glm::vec2 m_WorldPosition{};
	glm::vec2 m_WorldScale{};
	float m_WorldRotation{};
};


class Coordinator;
class TransformSystem : public System {

public:
	TransformSystem(Coordinator* const pRegistry);
	void Update(ComponentManager* const pComponentManager) override;
};