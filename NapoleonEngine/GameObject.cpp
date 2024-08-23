#include "PCH.h"
#include "GameObject.h"
#include "RendererComponent.h"
#include "Subject.h"

#include <algorithm>

#include "Scene.h"

GameObject::GameObject(Coordinator* const pRegistry)
	:m_pRegistry(pRegistry), 
	m_Entity(pRegistry->CreateEntity()),
	m_bIsActive(true),
	m_bIsDestroyed(false)
{
}

GameObject::GameObject(const GameObject& other)
	:m_Entity(m_pRegistry->CreateEntity()),
	m_bIsActive(true),
	m_bIsDestroyed(false)
{

}

GameObject::~GameObject()
{
	m_pRegistry->DestroyEntity(m_Entity);
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

ECS_TransformComponent* const GameObject::GetTransform() const
{ 
	return GetComponent<ECS_TransformComponent>(); 
}

void GameObject::AddChild(GameObject* const pChild)
{
	//m_pChildren.push_back(pChild); //remove ?
	//pChild->m_pParent = std::shared_ptr<GameObject>(this); //remove ?

	pChild->GetTransform()->SetParent(GetTransform());
	m_pRegistry->AddChild(m_Entity, pChild->m_Entity);

	//pChild->m_pScene = m_pScene; //remove ?
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
	writer.WriteString("Tag", GetTag());
	writer.StartArray("components");
	auto components = m_pRegistry->GetComponents(m_Entity);

	for (Component* const pComp : components)
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

	std::string tag;
	reader->ReadString("Tag", tag);
	SetTag(tag);
}

void GameObject::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	auto jsonComponents = reader->ReadArray("components");
	auto components = m_pRegistry->GetComponents(m_Entity);

	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->RestoreContext(jsonComponents->ReadArrayIndex(i).get(), context);
		components[i]->m_pGameObject = this;
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

void GameObject::SetActive(bool active, bool includeChildren)
{
	m_bIsActive = active;

	if (includeChildren)
	{
		m_pRegistry->SetEntityHierarchyActive(m_Entity, active);
	}
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