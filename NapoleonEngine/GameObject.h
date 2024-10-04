#pragma once
#include "Entity.h"
#include "Component.h"
#include "Registry.h"
#include "Serializer.h"

class TransformComponent;
class GameObject final
{
	friend class Scene;
public:
	~GameObject();

	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	template<class T> T* const GetComponent() const;
	template <class T> T* const GetComponentInChildren() const;
		
	TransformComponent* const GetTransform() const;

	template <typename T> T* const AddComponent();

	void AddChild(GameObject* const pChild);

	void SetActive(bool active, bool includeChildren = true);
	void Destroy();
		
	bool IsActive() const;
	std::string GetTag() const; //add compareTag function
	Entity GetEntity() const { return m_Entity; }
		
	void SetTag(std::string const& tag, bool applyToChildren = false);

private:
	friend class PrefabsManager;
	friend class BaseScene;

	GameObject(Entity entity, Registry* const pRegistry);
	GameObject(const GameObject& other);

	Registry* m_pRegistry;

	Entity const m_Entity;
};

template <typename T>
T* const GameObject::AddComponent()
{
	auto newComp = m_pRegistry->AddComponent<T>(m_Entity);
	if (newComp != nullptr)
	{
		newComp->m_pGameObject = this;
	}
	return newComp;
}

template <class T>
T* const GameObject::GetComponent() const
{
	return m_pRegistry->GetComponent<T>(m_Entity);
}

template <class T>
T* const GameObject::GetComponentInChildren() const
{
	return m_pRegistry->GetComponentInChildren<T>(m_Entity);
}