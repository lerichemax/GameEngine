#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#pragma warning(pop)

#include "Component.h"
#include "System.h"
#include "Scene.h"

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

//WIP
//To test
//TODO : make loc/rot/sca world and store another local
class ECS_TransformComponent final: public ECS_Component
{
	friend class TransformSystem;
	friend class GameObject;
	friend class Renderer;
	friend class Scene;

public:
	ECS_TransformComponent() : ECS_Component(true) {}
	~ECS_TransformComponent()
	{

	}
	//void SetLocation(vec2 const& loc);
	//void SetLocation(float x, float y);
	void SetLocalLocation(vec2 const& loc);
	//void SetLocalLocation(float x, float y);
	void Translate(vec2 const& translation);
	void Translate(float x, float y);
	void Scale(vec2 const& scale);
	void Scale(float scale);
	void Rotate(float rotation);

	const glm::vec2& GetPosition() const { return m_WorldPosition; }
	const glm::vec2& GetLocalPosition() const { return m_Position; }
	const glm::vec2& GetScale() const { return m_WorldScale; }
	const glm::vec2& GetLocalScale() const { return m_Scale; }
	float GetRotation() const { return m_WorldRotation; }
	float GetLocalRotation() const { return m_Rotation; }

	void SetParent(std::shared_ptr<ECS_TransformComponent> pParent);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

private:
	glm::vec2 m_Position{};
	glm::vec2 m_Scale{ 1.f, 1.f };
	float m_Rotation{};

	glm::mat3x3 m_World{};
	glm::vec2 m_WorldPosition{};
	glm::vec2 m_WorldScale{ 1.f, 1.f };
	float m_WorldRotation{};

	std::shared_ptr<ECS_TransformComponent> m_pParent;

	glm::vec2 m_OldPosition{};
	glm::vec2 m_OldScale{ 1.f, 1.f };
	float m_OldRotation{};

	bool HasChanged() const;
};


class Coordinator;
class TransformSystem : public System {
public:
	void Update(ComponentManager* const pComponentManager) override;

	void SetSignature(Coordinator* const pRegistry) override;

private:
	void RecursivelyUpdateHierarchy(std::shared_ptr<ECS_TransformComponent> transformComponent) const;
};