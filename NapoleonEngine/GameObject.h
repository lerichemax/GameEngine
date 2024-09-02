#pragma once
#include "Entity.h"
#include "Component.h"
#include "Coordinator.h"
#include "Serializer.h"

#include <vector>

class Subject;
class Observer;
class TransformComponent;
class TransformComponent;
class Scene;
class GameObject final : public IContextSerializable
{
	friend class Scene;
public:
	~GameObject();

	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	template <class T> bool HasComponent() const;
	template<class T> T* const GetComponent() const;
	template <class T> T* const GetComponentInChildren() const;
	template <class T> void RemoveComponent();
		
	TransformComponent* const GetTransform() const;

	template <typename T> T* const AddComponent();

	void AddChild(GameObject* const pChild);

	void SetActive(bool active, bool includeChildren = true);
	void Destroy();
		
	bool IsActive() const;
	std::string GetTag() const; //add compareTag function
	Entity GetEntity() const { return m_Entity; }
		
	void SetTag(std::string const& tag, bool applyToChildren = false);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

private:
	friend class PrefabsManager;
	friend class BaseScene;

	GameObject(Coordinator* const pRegistry);
	GameObject(const GameObject& other);

	Coordinator* m_pRegistry;

	Entity m_Entity;
		
	bool m_bIsActive;
	bool m_bIsDestroyed;
		
	void Refresh();
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

template <class T>
bool  GameObject::HasComponent() const
{
	//auto const& type = typeid(T);

	//for (Component* pObjectComp : m_pComponents)
	//{
	//	if (typeid(*pObjectComp) == type)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

template <class T>
void GameObject::RemoveComponent()
{
	//T* pComp = GetComponent<T>();

	//m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), pComp), m_pComponents.end());
	//delete pComp;
}