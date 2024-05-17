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
		
	glm::vec2 TransformIntoCameraSpace(glm::vec2 const pos);
	glm::mat3x3 GetCameraMatrix() const;
	
protected:
	void Initialize() override;
	void Update() override {};

	CameraComponent* Clone() const override { return new CameraComponent(*this); }
	
private:
		
	unsigned int m_Width;
	unsigned int m_Height;

	void Transform() const;
		
	CameraComponent(CameraComponent const& other);
};

struct ECS_CameraComponent : public ECS_Component
{
	ECS_CameraComponent();

	unsigned int m_Width{};
	unsigned int m_Height{};
};

class Coordinator;
class CameraSystem : public System
{
public:
	CameraSystem(Coordinator* const pRegistry);

	void Update(ComponentManager* const pComponentManager) override;

	bool TrySetMainCamera(std::shared_ptr<GameObject> pGameObject);

protected:
	std::shared_ptr<System> Clone() const override { return std::make_shared<CameraSystem>(*this); };

private:
	friend class Scene;

	Entity m_MainCameraEntity;
};