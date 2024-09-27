#include "PCH.h"
#include "Coordinator.h"

Coordinator::Coordinator()
	:m_pComponentManager{std::make_unique<ComponentManager>()},
	m_pEntityManager{ std::make_unique<EntityManager>() },
	m_pSystemManager{ std::make_unique<SystemManager>() }
{
}

Entity Coordinator::CreateEntity()
{
	return m_pEntityManager->CreateEntity();
}

void Coordinator::DestroyEntity(Entity entity)
{
	m_pEntityManager->DestroyEntity(entity);
	m_pComponentManager->EntityDestroyed(entity);
	m_pSystemManager->EntityDestroyed(entity);
}

void Coordinator::TransferTags(Entity originEntity, Entity destinationEntity, Coordinator* const pOther)
{
	if (pOther->m_pEntityManager->HasATag(originEntity))
	{
		m_pEntityManager->SetTag(destinationEntity, pOther->GetTag(originEntity));
	}
}

void Coordinator::DeserializeComponents(Entity entity, JsonReader const* reader /*array*/, SerializationMap& context)
{
	Signature signature = m_pEntityManager->GetSignature(entity);
	for (SizeType i = 0; i < reader->GetArraySize(); i++)
	{
		auto arrayIndex = reader->ReadArrayIndex(i);
		ComponentType type = m_pComponentManager->DeserializeAndAddComponent(entity, arrayIndex.get(), context);
		signature.set(type, true);
	}
	
	m_pEntityManager->SetSignature(entity, signature);
	m_pSystemManager->EntitySignatureChanged(entity, signature);
}

System* const Coordinator::AddSystemFromHash(size_t type)
{
	auto* const pSystem = Factory<System>::Get().Create(type);

	if (m_pSystemManager->ForceAddSystem(type, pSystem))
	{
		System* const pAddedSystem = m_pSystemManager->GetSystemFromType(type);
		pAddedSystem->m_pRegistry = this;
		pAddedSystem->SetSignature();
		return pAddedSystem;
	}
	else
	{
		delete pSystem;
		return nullptr;
	}
}

void Coordinator::SetEntityActive(Entity entity, bool isActive)
{
	auto components = m_pComponentManager->GetComponentsForSignature(entity, m_pEntityManager->GetSignature(entity));

	for (Component* const comp : components)
	{
		comp->SetActive(isActive);
	}
}

void Coordinator::SetEntityHierarchyActive(Entity entity, bool isActive)
{
	SetEntityActive(entity, isActive);

	std::unordered_set<Entity> const& children = m_pEntityManager->GetChildren(entity);

	for (Entity child : children)
	{
		SetEntityActive(child, isActive);
	}
}

void Coordinator::AddChild(Entity parent, Entity child)
{
	m_pEntityManager->AddChild(parent, child);
}

void Coordinator::SetTag(Entity entity, std::string const& tag)
{
	m_pEntityManager->SetTag(entity, tag);
}

std::string Coordinator::GetTag(Entity entity) const
{
	return m_pEntityManager->GetTag(entity);
}

bool Coordinator::HasTag(Entity entity, std::string const& tag) const
{
	return m_pEntityManager->HasTag(entity, tag);
}

std::unordered_set<Entity> const& Coordinator::GetChildren(Entity entity) const
{
	return m_pEntityManager->GetChildren(entity);
}

std::vector<Entity> Coordinator::GetEntityHierarchy(Entity entity) const
{
	return m_pEntityManager->GetEntityHierarchy(entity);
}

std::vector<Component*> Coordinator::GetEntityComponents(Entity entity)
{
	return m_pComponentManager->GetComponentsForSignature(entity, m_pEntityManager->GetSignature(entity));
}