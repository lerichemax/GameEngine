#pragma once
#include "Entity.h"
#include "Component.h"
#include "Coordinator.h"
#include "Serializer.h"

#include <vector>

class Subject;
class Observer;
class TransformComponent;
class ECS_TransformComponent;
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
	template<class T> std::shared_ptr<T> GetComponent() const;
	template <class T> T* GetComponentInChildren() const;
	template <class T> void RemoveComponent();
		
	std::shared_ptr<ECS_TransformComponent> GetTransform() const { return m_pTransform; }

	template <typename T> void AddComponent(T const& Component);

	void AddChild(std::shared_ptr<GameObject> pChild);

	void SetActive(bool active);
	void Destroy();
		
	Scene* const GetParentScene() const { return m_pScene; }
		
	bool IsActive() const;
	std::string GetTag() const; //add compareTag function
	Entity GetEntity() const { return m_Entity; }
		
	void AddObserver(Observer* pObserver) const;
	void RemoveObserver(Observer* pObserver) const;
	void Notify(int event);
	void SetTag(std::string const& tag, bool applyToChildren = false);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

private:
	friend class PrefabsManager;
	friend class BaseScene;

	GameObject(std::weak_ptr<Coordinator> pRegistry, bool transform = true);
	GameObject(const GameObject& other);

	std::shared_ptr<Coordinator> m_pRegistry;

	Entity m_Entity;
		
	bool m_bIsActive;
	bool m_bIsDestroyed;

	std::shared_ptr <ECS_TransformComponent> m_pTransform;
		
	Scene* m_pScene;
	Subject* m_pSubject;
		
	void Refresh();
	void Update();
};
template <typename T>
void GameObject::AddComponent(T const& Component)
{
	auto newComp = m_pRegistry->AddComponent<T>(m_Entity, Component);
	newComp->m_pGameObject = std::shared_ptr<GameObject>(this);
}

template <class T>
std::shared_ptr<T> GameObject::GetComponent() const
{
	return m_pRegistry->GetComponent<T>(m_Entity).lock();
}

template <class T>
T* GameObject::GetComponentInChildren() const
{
	/*T* pComp{};
	for (auto pChild : m_pChildren)
	{
		pComp = pChild->GetComponent<T>();
		if (pComp != nullptr)
		{
			return pComp;
		}
	}*/
	return nullptr;
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