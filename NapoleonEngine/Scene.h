#pragma once
#include "SceneManager.h"
#include "ColliderComponent.h"
#include "Registry.h"
#include "Entity.h"

#include <memory>
#include <unordered_set>

class SceneRenderer;
class GameObject;
class CameraSystem;
class TransformSystem;
class LayeredRendererSystem;
class TextRendererSystem;
class System;
class AudioSystem;
class UiSystem;
class CollisionSystem;
class BaseScene : public IContextSerializable
{
	friend class NapoleonEngine;
	template <SystemDerived T> friend T* const GetSystem();

public:
	explicit BaseScene(const std::string& name);
	virtual ~BaseScene();

	virtual [[nodiscard]] std::shared_ptr<GameObject> CreateGameObject();
	std::string GetName() const { return m_Name; }

	void Serialize(StreamWriter& writer) const override;
	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

protected:
	BaseScene();

	std::unique_ptr<Registry> m_pRegistry;

	std::string m_Name;

	std::shared_ptr<GameObject> CreateGameObjectNoTransform();

};

class Prefab : public BaseScene
{
	friend class PrefabsManager;
	friend class Scene;
public:
	explicit Prefab();

	std::shared_ptr<GameObject> CreateGameObject() override;
	std::shared_ptr<GameObject> GetRoot() const;

	template <SystemDerived T> void AddRequiredSystem();

	void Serialize(StreamWriter& writer) const override;

protected:
	void SetName(std::string const& name);

private:
	Entity m_pRootEntity;
	std::unordered_set<size_t> m_RequiredSystems;
};

template <SystemDerived T> 
void Prefab::AddRequiredSystem() 
{
	size_t type = std::type_index(typeid(T)).hash_code();

	assert(m_RequiredSystems.find(type) == m_RequiredSystems.end() && std::string("System with Hash " + std::to_string(type) + " already added as required.").c_str());

	m_pRegistry->RegisterSystem<T>();
	m_RequiredSystems.insert(type);
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

	template <SystemDerived T> T* const AddSystem();
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

protected:
	std::shared_ptr<GameObject> GetCameraObject() const;

	Color m_BackgroundColor{ 0,0,0,0 };

private:
	friend class SceneManager;
	friend class NapoleonEngine;
	friend std::shared_ptr<GameObject> Instantiate(std::string const& name);
	friend std::shared_ptr<GameObject> Instantiate(std::string const& name, glm::vec2 const& location);
	template <ComponentDerived T> friend T* const FindComponentOfType();
		
	void AddCollider(ColliderComponent* pCollider);
	void RemoveCollider(ColliderComponent* pCollider);
		
	std::vector<ColliderComponent*> m_pColliders;
		
	TextRendererSystem* m_pTextRenderer;
	LayeredRendererSystem* m_pLayeredRenderer;
	TransformSystem* m_pTransformSystem;
	CollisionSystem* m_pCollisionSystem;
	AudioSystem* m_pAudio;
	CameraSystem* m_pCameraSystem;
	UiSystem* m_pUi;

	Entity m_CameraEntity;
	std::vector<System*> m_pSystems;

	bool m_bIsActive;
	bool m_bIsInitialized;

	virtual void Initialize() {};
	void Update();
		
	void OnLoad();
	void CleanUpScene();
	std::shared_ptr<GameObject> InstantiatePrefab(std::string const& name);
	std::shared_ptr<GameObject> InstantiatePrefab(std::string const& name, glm::vec2 const& location);
};

template <SystemDerived T>
T* const Scene::AddSystem()
{
	auto* const pNewSystem = m_pRegistry->RegisterSystem<T>();
	m_pSystems.push_back(pNewSystem);
	return pNewSystem;
}
