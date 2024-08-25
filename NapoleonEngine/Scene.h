#pragma once
#include "SceneManager.h"
#include "ColliderComponent.h"
#include "Coordinator.h"
#include "Entity.h"

#include <memory>
#include <unordered_set>

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
class BehaviourSystem;
class UiSystem;

class BaseScene : public IContextSerializable
{
	friend class NapoleonEngine;
	template <SystemDerived T> friend T* const GetSystem();

public:
	explicit BaseScene(const std::string& name);
	virtual ~BaseScene();

	virtual GameObject* const CreateGameObject();
	std::string GetName() const { return m_Name; }

	GameObject* const FindTagInChildren(GameObject* const pObj, std::string const& tag) const; //returns the first one found
	std::vector<GameObject*> GetChildrenWithTag(GameObject* const pObj, std::string const& tag) const;

	void Serialize(StreamWriter& writer) const override;
	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

protected:
	BaseScene();

	std::unique_ptr<Coordinator> m_pRegistry;

	std::string m_Name;
	std::vector<std::unique_ptr<GameObject>> m_pObjects{};

	GameObject* const CreateGameObjectNoTransform();

private:
	GameObject* const GetGameObjectWithEntity(Entity entity) const;
};

class Prefab : public BaseScene
{
	friend class PrefabsManager;
	friend class Scene;
public:
	explicit Prefab();

	GameObject* const CreateGameObject() override;
	GameObject* const GetRoot() const;

	bool IsRoot(GameObject* pObject) const;
	template <SystemDerived T> void AddRequiredSystem();

	void Serialize(StreamWriter& writer) const override;

protected:
	void SetName(std::string const& name);

private:
	GameObject* const m_pRootObject;
	std::unordered_set<std::string> m_RequiredSystems;
};

template <SystemDerived T> 
void Prefab::AddRequiredSystem() 
{
	std::string typeName = typeid(T).name();

	assert(m_RequiredSystems.find(typeName) == m_RequiredSystems.end() && std::string(typeName + " already added as required.").c_str());

	m_RequiredSystems.insert(typeName);
}

class ShapeRenderer;
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

	template <typename T> void AddSystem();
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

protected:
	void SetActiveCamera(GameObject* const pGameObject);
	GameObject* const GetCameraObject() const;

	Color m_BackgroundColor{ 0,0,0,0 };

private:
	friend void ColliderComponent::Initialize();
	friend ColliderComponent::~ColliderComponent();
	friend class SceneManager;
	friend class NapoleonEngine;
	friend GameObject* const Instantiate(std::string const& name);
	friend GameObject* const Instantiate(std::string const& name, glm::vec2 const& location);
	template <ComponentDerived T> friend T* const FindComponentOfType();
		
	void AddCollider(ColliderComponent* pCollider);
	void RemoveCollider(ColliderComponent* pCollider);
		
	std::vector<ColliderComponent*> m_pColliders;
		
	TextRendererSystem* m_pTextRenderer;
	LayeredRendererSystem* m_pLayeredRenderer;
	ShapeRenderer* m_pShapeRenderer;
	TransformSystem* m_pTransformSystem;
	AudioSystem* m_pAudio;
	CameraSystem* m_pCamera;
	BehaviourSystem* m_pBehaviours;
	UiSystem* m_pUi;

	GameObject* m_pCameraObject;
	std::vector<System*> m_pSystems;

	bool m_bIsActive;
	bool m_bIsInitialized;

	virtual void Initialize() {};
	void Update();
	virtual void DeclareInput() {};
		
	void OnLoad();
	void Refresh();
	void CleanUpScene();
	void CheckCollidersCollision();
	GameObject* const InstantiatePrefab(std::string const& name);
	GameObject* const InstantiatePrefab(std::string const& name, glm::vec2 const& location);
};

template <typename T>
void Scene::AddSystem()
{
	m_pSystems.push_back(m_pRegistry->RegisterSystem<T>());
}
