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

	template <class T> bool HasComponent() const;
	template<class T> std::shared_ptr<T> GetComponent() const;
	template <class T> T* GetComponentInChildren() const;
	template <class T> void RemoveComponent();
		
	std::shared_ptr<ECS_TransformComponent> GetTransform() const { return m_pTransform; }

	template <typename T> void AddComponent(T const& Component);

	void AddChild(std::shared_ptr<GameObject> pChild);
	std::vector<std::shared_ptr<GameObject>> const& GetChildren() const { return m_pChildren; }
		
	void SetActive(bool active);
	void Destroy();
		
	std::shared_ptr<GameObject> GetParent() const{ return m_pParent; }
	Scene* const GetParentScene() const { return m_pScene; }
		
	bool IsActive() const;
	bool HasChildren() const { return m_pChildren.size() > 0; }
	std::string GetTag()const { return m_Tag; };
	Entity GetEntity() const { return m_Entity; }
		
	void AddObserver(Observer* pObserver) const;
	void RemoveObserver(Observer* pObserver) const;
	void Notify(int event);
	void SetTag(std::string const& tag, bool applyToChildren);
	std::shared_ptr<GameObject> FindTagInChildren(std::string const& tag);

private:
	friend class PrefabsManager;
	friend class BaseScene;

	GameObject(std::weak_ptr<Coordinator>);
	GameObject(const GameObject& other);

	std::shared_ptr<Coordinator> m_pRegistry;

	Entity m_Entity;
		
	bool m_bIsActive;
	bool m_bIsDestroyed;
		
	std::vector<Component*> m_pComponents;

	std::shared_ptr <ECS_TransformComponent> m_pTransform;

	std::vector<std::shared_ptr<GameObject>> m_pChildren;
	std::shared_ptr<GameObject> m_pParent;
		
	Scene* m_pScene;
	Subject* m_pSubject;

	std::string m_Tag;
		
	void Refresh();
	void Update();
};
template <typename T>
void GameObject::AddComponent(T const& Component)
{
	m_pRegistry->AddComponent<T>(m_Entity, Component);
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