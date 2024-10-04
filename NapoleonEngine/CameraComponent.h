#pragma once
#include "Component.h"
#include "System.h"
#include "ComponentManager.h"

class CameraComponent final : public Component
{
public:
	CameraComponent();
	CameraComponent(CameraComponent&& other) = delete;
	CameraComponent& operator=(CameraComponent const& rhs) = delete;
	CameraComponent& operator=(CameraComponent&& rhs) = delete;
	~CameraComponent() = default;
		
	glm::vec2 TransformIntoCameraSpace(glm::vec2 const Pos);
	glm::mat3x3 GetCameraMatrix() const;
	
protected:
	void Initialize();
	
private:
		
	unsigned int m_Width;
	unsigned int m_Height;

	void Transform() const;
		
	CameraComponent(CameraComponent const& other);
};

struct ECS_CameraComponent : public Component
{
public:
	ECS_CameraComponent();

	unsigned int m_Width{};
	unsigned int m_Height{};

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {};
};

class Registry;
class CameraSystem : public System
{
public:
	void Update() override;

	bool TrySetMainCamera(GameObject* const pGameObject);

	void SetSignature() const override;

private:
	friend class Scene;

	Entity m_MainCameraEntity;
};