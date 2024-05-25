#pragma once
#include "SceneManager.h"
#include "ColliderComponent.h"
#include "Coordinator.h"
#include "Entity.h"

#include <memory>

enum class Layer;
class SceneRenderer;
class RendererComponent;
class GameObject;
class CameraComponent;
class CameraSystem;
class TransformSystem;
class LayeredRendererSystem;
class TextRendererSystem;
class System;

class BaseScene
{
	friend class NapoleonEngine;

public:
	explicit BaseScene(const std::string& name);
	virtual ~BaseScene();

	std::shared_ptr<GameObject> CreateGameObject();
	std::string GetName() const { return m_Name; }

	std::shared_ptr<GameObject> FindTagInChildren(std::shared_ptr<GameObject> pObj, std::string const& tag) const; //returns the first one found
	std::vector<std::shared_ptr<GameObject>> GetChildrenWithTag(std::shared_ptr<GameObject> pObj, std::string const& tag) const;

protected:
	std::shared_ptr<Coordinator> m_pRegistry;

	std::string m_Name;
	std::vector<std::shared_ptr<GameObject>> m_pObjects;
	std::vector<std::shared_ptr<System>> m_Systems;

	template <typename T> void AddSystem();
	template <typename T> void AddSystem(Signature signature);
	std::shared_ptr<GameObject> InstantiatePrefab(std::string const& name);

private:
	std::shared_ptr<GameObject> GetGameObjectWithEntity(Entity entity) const;
};

class Scene : public BaseScene
{
public:
	explicit Scene(const std::string& name);
	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;
		
	void Render() const;
	bool IsActive() const { return m_bIsActive; }
	void AddToGroup(RendererComponent* pRenderer, Layer layer) const;
	void RemoveFromGroup(RendererComponent* pRenderer, Layer layer) const;
	void SetCameraActive(CameraComponent* pCamera) { m_pActiveCamera = pCamera; }

protected:
	virtual void CustomOnActivate(){}
	void SetActiveCamera(std::shared_ptr<GameObject> pGameObject);
	std::shared_ptr<GameObject> GetCameraObject() const;

private:
	friend void ColliderComponent::Initialize();
	friend ColliderComponent::~ColliderComponent();
	friend class SceneManager;
	friend class NapoleonEngine;
		
	void AddCollider(ColliderComponent* pCollider);
	void RemoveCollider(ColliderComponent* pCollider);
		
	std::vector<ColliderComponent*> m_pColliders;
		
	SceneRenderer* m_pSceneRenderer; //deprecated
	std::shared_ptr<TextRendererSystem> m_pTextRenderer;
	std::shared_ptr<LayeredRendererSystem> m_pECS_SceneRenderer;
	std::shared_ptr<TransformSystem> m_pTransformSystem;
	std::shared_ptr<CameraSystem> m_pCamera;
	std::shared_ptr<GameObject> m_pCameraObject;
	CameraComponent* m_pActiveCamera; //deprecated

	bool m_bIsActive;
	bool m_bIsInitialized;

	virtual void Initialize() {};
	void Update();
	virtual void DeclareInput() {};
		
	void OnActivate();
	void Refresh();
	void CleanUpScene();
	void CheckCollidersCollision();
};

template <typename T>
void BaseScene::AddSystem()
{
	m_Systems.push_back(m_pRegistry->RegisterSystem<T>());
}

template <typename T>
void BaseScene::AddSystem(Signature signature)
{
	m_Systems.push_back(m_pRegistry->RegisterSystem<T>(signature));
}