#pragma once
#include "SceneManager.h"
#include "ColliderComponent.h"
#include "Registry.h"
#include "Entity.h"

#include <memory>
#include <unordered_set>

class System;
class GameObject;
class TransformSystem;
class AudioSystem;
class UiSystem;
class CollisionSystem;
class Camera2D;
class BaseScene : public IContextSerializable
{
	friend class NapoleonEngine;

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
	std::shared_ptr<GameObject> GetRoot() const;

protected:
	void SetName(std::string const& name);

private:
	Entity m_pRootEntity;
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
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
	Color const& GetBackgroundColor() const;

protected:
	Color m_BackgroundColor{ 0,0,0,0 };

	Camera2D* const GetCamera() const { return m_pCamera.get(); }

private:
	friend class SceneManager;
	friend class NapoleonEngine;
	friend std::shared_ptr<GameObject> Instantiate(std::string const& name);
	friend std::shared_ptr<GameObject> Instantiate(std::string const& name, glm::vec2 const& location);
	template <ComponentDerived T> friend T* const FindComponentOfType();
		
	void AddCollider(ColliderComponent* pCollider);
	void RemoveCollider(ColliderComponent* pCollider);
		
	std::vector<ColliderComponent*> m_pColliders;
		
	TransformSystem* m_pTransformSystem;
	CollisionSystem* m_pCollisionSystem;
	AudioSystem* m_pAudio;
	UiSystem* m_pUi;

	std::unique_ptr<Camera2D> m_pCamera;

	std::vector<ecs::System*> m_pSystems;

	bool m_bIsActive;
	bool m_bIsInitialized;

	virtual void Initialize() {};
	void Update();
		
	void OnLoad();
	void CleanUpScene();

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
