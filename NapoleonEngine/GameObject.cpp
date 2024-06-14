#include "PCH.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "Subject.h"

#include <algorithm>

#include "Scene.h"

GameObject::GameObject(std::weak_ptr<Coordinator> pRegistry, bool transform)
	:m_pRegistry(pRegistry), 
	m_Entity(pRegistry.lock()->CreateEntity()),
	m_bIsActive(true),
	m_bIsDestroyed(false),
	m_pTransform(nullptr),
	m_pScene(nullptr),
	m_pSubject(new Subject{}),
	m_Tag()
{
	if (transform)
	{
		ECS_TransformComponent transformComp;
		m_pRegistry->AddComponent<ECS_TransformComponent>(m_Entity, transformComp);
		m_pTransform = m_pRegistry->GetComponent<ECS_TransformComponent>(m_Entity).lock();
	}
}

GameObject::GameObject(const GameObject& other)
	:m_Entity(m_pRegistry->CreateEntity()),
	m_bIsActive(true),
	m_bIsDestroyed(false),
	m_pScene(nullptr),
	m_pSubject(new Subject{*other.m_pSubject}),
	m_Tag(other.m_Tag)
{

}

GameObject::~GameObject()
{
	m_pRegistry->DestroyEntity(m_Entity);

	SafeDelete(m_pSubject);
}

void GameObject::Update()
{

}

void GameObject::Refresh()
{
	
	//for (auto& pChild : m_pChildren)
	//{
	//	pChild->Refresh();
	//	if (pChild->m_bIsDestroyed)
	//	{
	//		SafeDelete(pChild);
	//	}
	//}
}

void GameObject::AddChild(std::shared_ptr<GameObject> pChild)
{
	//m_pChildren.push_back(pChild); //remove ?
	//pChild->m_pParent = std::shared_ptr<GameObject>(this); //remove ?

	pChild->m_pTransform->m_pParent = m_pTransform;
	m_pRegistry->AddChild(m_Entity, pChild->m_Entity);

	//pChild->m_pScene = m_pScene; //remove ?
}

void GameObject::AddObserver(Observer* pObserver) const
{
	m_pSubject->AddObserver(pObserver);
}

void GameObject::RemoveObserver(Observer* pObserver) const
{
	m_pSubject->RemoveObserver(pObserver);
}

void GameObject::Notify(int event)
{
	m_pSubject->Notify(this, event);
}

void GameObject::SetTag(std::string const& tag, bool applyToChildren)
{
	m_pRegistry->SetTag(m_Entity, tag);

	if (!applyToChildren)
	{
		return;
	}

	std::unordered_set<Entity> children = m_pRegistry->GetChildren(GetEntity());

	for (Entity entity : children)
	{
		m_pRegistry->SetTag(entity, tag);
	}
}

void GameObject::Serialize(StreamWriter& writer) const
{ 
	writer.WriteInt("Entity", m_Entity);
	writer.StartArray("components");
	auto components = m_pRegistry->GetComponents(m_Entity);

	for (std::shared_ptr<ECS_Component> pComp : components)
	{
		writer.StartArrayObject();
		pComp->Serialize(writer);
		writer.EndObject();
	}
	writer.EndArray();

	auto children = m_pRegistry->GetChildren(m_Entity);
	writer.StartArray("children");
	for (Entity entity : children)
	{
		writer.WriteIntNoKey(static_cast<int>(entity));
	}
	writer.EndArray();
}

void GameObject::Deserialize(JsonReader const* reader, SerializationMap& context)
{ 
	m_pRegistry->DeserializeComponents(m_Entity, reader->ReadArray("components").get(), context);
	int entity;
	reader->ReadInt("Entity", entity);
	context.Add(entity, this);
}

void GameObject::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	auto components = m_pRegistry->GetComponents(m_Entity);
	for (std::shared_ptr<ECS_Component> pComp : components)
	{
		pComp->RestoreContext(reader, context);
	}

	auto children = reader->ReadArray("children");
	if (children != nullptr && children->IsValid())
	{
		for (SizeType i = 0; i < children->GetArraySize(); i++)
		{
			int child = children->ReadArrayIndexAsInt(i);
			m_pRegistry->AddChild(m_Entity, context.GetRef<GameObject>(child)->m_Entity);
		}
	}
}

void GameObject::SetActive(bool active)
{
	m_bIsActive = active;

	m_pRegistry->SetEntityHierarchyActive(m_Entity, active);
}

bool GameObject::IsActive() const
{
	return m_bIsActive;//&& m_pScene->IsActive();
}

std::string GameObject::GetTag() const
{ 
	return m_pRegistry->GetTag(m_Entity); 
}

void GameObject::Destroy()
{
	m_bIsDestroyed = true;
	m_bIsActive = false;
}