#pragma once
#include "Entity.h"
#include "ComponentArray.h"
#include "SystemManager.h"
#include "Registry.h"

#include <memory>
#include <unordered_set>

namespace ecs
{
	class System;
}

class GameObject;
class TransformSystem;
class AudioSystem;
class UiSystem;
class CollisionSystem;
class Camera2D;
class ColliderComponent;
class BaseScene
{
	friend class NapoleonEngine;
	template<ecs::ComponentDerived C> friend C* CreateComponent();
	template <ecs::ComponentDerived T> friend T* const FindComponentOfType();
public:
	explicit BaseScene(const std::string& name);
	virtual ~BaseScene();

	virtual [[nodiscard]] std::shared_ptr<GameObject> CreateGameObject();
	std::string GetName() const { return m_Name; }

	void RestoreContext(JsonReader* const reader, SerializationMap const& context);
	void Deserialize(JsonReader* const reader, SerializationMap& context);

protected:
	BaseScene();

	std::unique_ptr<ecs::Registry> m_pRegistry;

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
	void Serialize(StreamWriter& writer) const;

protected:
	void SetName(std::string const& name);

private:
	ecs::Entity m_pRootEntity;
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
		
	bool IsActive() const { return m_bIsActive; }

	template <ecs::SystemDerived T> T* const AddSystem();
	Color const& GetBackgroundColor() const;

protected:
	Color m_BackgroundColor{ 0,0,0,0 };

	virtual void Initialize() = 0;

private:
	friend class SceneManager;
	friend class NapoleonEngine;
	friend std::shared_ptr<GameObject> Instantiate(std::string const& name);
	friend std::shared_ptr<GameObject> Instantiate(std::string const& name, glm::vec2 const& location);
		
	void AddCollider(ColliderComponent* pCollider);
	void RemoveCollider(ColliderComponent* pCollider);
		
	std::vector<ColliderComponent*> m_pColliders;
		
	TransformSystem* m_pTransformSystem;
	CollisionSystem* m_pCollisionSystem;
	AudioSystem* m_pAudio;
	UiSystem* m_pUi;

	std::unique_ptr<Camera2D> m_pCamera;
	std::shared_ptr<PrefabsManager> m_pPrefabsManager;

	std::vector<ecs::System*> m_pSystems;

	bool m_bIsActive;
	bool m_bIsInitialized;

	void Update();
		
	void OnLoad();
	void CleanUpScene();
	void AddFPSCounter();

	std::shared_ptr<GameObject> InstantiatePrefab(std::string const& name);
	std::shared_ptr<GameObject> InstantiatePrefab(std::string const& name, glm::vec2 const& location);
};

template <ecs::SystemDerived T>
T* const Scene::AddSystem()
{
	auto* const pNewSystem = m_pRegistry->RegisterSystem<T>();
	m_pSystems.push_back(pNewSystem);
	return pNewSystem;
}
