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

TransformComponent* const GameObject::GetTransform() const
{ 
	return GetComponent<TransformComponent>(); 
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
	writer.WriteInt(std::string{ "Entity" }, m_Entity);
	writer.WriteString(std::string{ "Tag" }, GetTag());
	writer.StartArray(std::string{ "components" });
	auto components = m_pRegistry->GetEntityComponents(m_Entity);

	for (Component* const pComp : components)
	{
		writer.StartArrayObject();
		pComp->Serialize(writer);
		writer.EndObject();
	}
	writer.EndArray();

	auto children = m_pRegistry->GetChildren(m_Entity);
	writer.StartArray(std::string{ "children" });
	for (Entity entity : children)
	{
		writer.WriteIntNoKey(static_cast<int>(entity));
	}
	writer.EndArray();
}

void GameObject::Deserialize(JsonReader const* reader, SerializationMap& context)
{ 
	m_pRegistry->DeserializeComponents(m_Entity, reader->ReadArray(std::string{ "components" }).get(), context);

	std::string tag;
	reader->ReadString(std::string{ "Tag" }, tag);
	SetTag(tag);
}

void GameObject::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	auto jsonComponents = reader->ReadArray(std::string{ "components" });
	auto components = m_pRegistry->GetEntityComponents(m_Entity);

	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->RestoreContext(jsonComponents->ReadArrayIndex(i).get(), context);
		components[i]->m_pGameObject = this;
	}

	auto children = reader->ReadArray(std::string{ "children" });
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
	else
	{
		m_pRegistry->SetEntityActive(m_Entity, active);
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