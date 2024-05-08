#pragma once
#include "Entity.h"
#include "Component.h"
#include "Coordinator.h"

#include <vector>

class Subject;
class Observer;
class TransformComponent;
struct ECS_TransformComponent;
class Scene;
class GameObject final
{
	friend class Scene;
public:
	GameObject();
	~GameObject();

	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	template <class T>
	bool HasComponent() const;

	template<class T>
	std::shared_ptr<T> GetComponent() const;

	template <class T>
	T* GetComponentInChildren() const;

	template <class T>
	void RemoveComponent();
		
	//TransformComponent* GetECSTransform() const { return m_pTransform; } //deprecated
	std::shared_ptr<ECS_TransformComponent> GetECSTransform() const { return m_pEcsTransform; }

	void AddComponent(Component* pComp); //deprecated
	template <typename T> void AddComponent(T const& Component, bool n /*temp*/);

	void AddChild(GameObject* pChild);
	std::vector<GameObject*> const& GetChildren() const { return m_pChildren; }
		
	void SetActive(bool active);
	void Destroy();
		
	GameObject* GetParent() const{ return m_pParent; }
	Scene* const GetParentScene() const { return m_pScene; }
		
	bool IsInitialized() const { return m_bIsInitialized; }
	bool IsActive() const;
	bool HasChildren() const { return m_pChildren.size() > 0; }
	std::string GetTag()const { return m_Tag; };
	Entity GetEntity() const { return m_Entity; }
		
	void AddObserver(Observer* pObserver) const;
	void RemoveObserver(Observer* pObserver) const;
	void Notify(int event);
	void SetTag(std::string const& tag, bool applyToChildren);
	GameObject* FindTagInChildren(std::string const& tag);

private:
	friend class PrefabsManager;
	friend class Scene;

	GameObject(std::weak_ptr<Coordinator>);

	std::shared_ptr<Coordinator> m_pRegistry;

	Entity m_Entity;
		
	bool m_bIsActive;
	bool m_bIsDestroyed;
	bool m_bIsInitialized;
		
	std::vector<Component*> m_pComponents;

	TransformComponent* m_pTransform;
	std::shared_ptr <ECS_TransformComponent> m_pEcsTransform;

	std::vector<GameObject*> m_pChildren;
	GameObject* m_pParent;
		
	Scene* m_pScene;
	Subject* m_pSubject;

	std::string m_Tag;
		
	void Refresh();
	void Initialize();
	void Update();
		
	GameObject(const GameObject& other);
};
template <typename T>
void GameObject::AddComponent(T const& Component, bool n)
{
	if (n) //TEMP
	{
		m_pRegistry->AddComponent<T>(m_Entity, Component);
	}
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
	auto const& type = typeid(T);

	for (Component* pObjectComp : m_pComponents)
	{
		if (typeid(*pObjectComp) == type)
		{
			return true;
		}
	}
	return false;
}

template <class T>
void GameObject::RemoveComponent()
{
	T* pComp = GetComponent<T>();

	m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), pComp), m_pComponents.end());
	delete pComp;
}