#pragma once
#include "SceneManager.h"
#include "ColliderComponent.h"
#include "Coordinator.h"
#include "Entity.h"

#include <memory>

enum class Layer : uint8_t;
class SceneRenderer;
class RendererComponent;
class GameObject;
class CameraComponent;
class CameraSystem;
class TransformSystem;
class LayeredRendererSystem;
class TextRendererSystem;
class System;
class AudioSystem;

class BaseScene : public IContextSerializable
{
	friend class NapoleonEngine;

public:
	explicit BaseScene(const std::string& name);
	virtual ~BaseScene();

	virtual std::shared_ptr<GameObject> CreateGameObject();
	std::string GetName() const { return m_Name; }

	std::shared_ptr<GameObject> FindTagInChildren(std::shared_ptr<GameObject> pObj, std::string const& tag) const; //returns the first one found
	std::vector<std::shared_ptr<GameObject>> GetChildrenWithTag(std::shared_ptr<GameObject> pObj, std::string const& tag) const;

	void Serialize(StreamWriter& writer) const override;
	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

protected:
	BaseScene();

	std::shared_ptr<Coordinator> m_pRegistry;

	std::string m_Name;
	std::vector<std::shared_ptr<GameObject>> m_pObjects;
	std::vector<std::shared_ptr<System>> m_pSystems;

	template <typename T> void AddSystem();
	template <typename T> void AddSystem(Signature signature);

	std::shared_ptr<GameObject> CreateGameObjectNoTransform();

private:
	std::shared_ptr<GameObject> GetGameObjectWithEntity(Entity entity) const;
};

class Prefab : public BaseScene
{
	friend class PrefabsManager;
	friend class Scene;
public:
	explicit Prefab();

	std::shared_ptr<GameObject> CreateGameObject() override;
	std::shared_ptr<GameObject> GetRoot() const;

	bool IsRoot(std::shared_ptr<GameObject> pObject) const;

protected:
	void SetName(std::string const& name);

private:
	std::shared_ptr<GameObject> m_pRootObject;
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
	std::shared_ptr<GameObject> InstantiatePrefab(std::string const& name);

private:
	friend void ColliderComponent::Initialize();
	friend ColliderComponent::~ColliderComponent();
	friend class SceneManager;
	friend class NapoleonEngine;
	friend std::shared_ptr<GameObject> Instantiate(std::string const& name);
		
	void AddCollider(ColliderComponent* pCollider);
	void RemoveCollider(ColliderComponent* pCollider);
		
	std::vector<ColliderComponent*> m_pColliders;
		
	SceneRenderer* m_pSceneRenderer; //deprecated
	std::shared_ptr<TextRendererSystem> m_pTextRenderer;
	std::shared_ptr<LayeredRendererSystem> m_pECS_SceneRenderer;
	std::shared_ptr<TransformSystem> m_pTransformSystem;
	std::shared_ptr<AudioSystem> m_pAudio;
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
	std::shared_ptr<GameObject> MergePrefab(std::shared_ptr<Prefab> pPrefab);
};

template <typename T>
void BaseScene::AddSystem()
{
	m_pSystems.push_back(m_pRegistry->RegisterSystem<T>());
}

template <typename T>
void BaseScene::AddSystem(Signature signature)
{
	m_pSystems.push_back(m_pRegistry->RegisterSystem<T>(signature));
}