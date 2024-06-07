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

ComponentManager* const Coordinator::GetComponentManager() const
{
	return m_pComponentManager.get();
}

void Coordinator::TransferComponents(Entity originEntity, Entity destinationEntity, std::shared_ptr<Coordinator> pOther)
{
	m_pEntityManager->SetSignature(destinationEntity, pOther->m_pEntityManager->GetSignature(originEntity));

	Signature newSignature = m_pEntityManager->GetSignature(destinationEntity);

	for (std::pair<const char*, ComponentType> const& pair : m_pComponentManager->m_ComponentTypes)
	{
		if (newSignature[pair.second] == 1)
		{
			if (m_pComponentManager->m_ComponentArrays.find(pair.first) == m_pComponentManager->m_ComponentArrays.end())
			{
				m_pComponentManager->m_ComponentArrays.insert(std::make_pair(pair.first, pOther->m_pComponentManager->m_ComponentArrays.at(pair.first)->Clone()));
			}

			m_pComponentManager->m_ComponentArrays.at(pair.first)->CloneToNewEntity(pOther->m_pComponentManager->m_ComponentArrays.at(pair.first), destinationEntity, originEntity);
		}
	}

	m_pSystemManager->EntitySignatureChanged(destinationEntity, newSignature);
}

void Coordinator::TransferTags(Entity originEntity, Entity destinationEntity, std::shared_ptr<Coordinator> pOther)
{
	if (pOther->m_pEntityManager->HasATag(originEntity))
	{
		m_pEntityManager->SetTag(destinationEntity, pOther->GetTag(originEntity));
	}
}

void Coordinator::DeserializeComponents(Entity entity, JsonReader const* reader /*array*/, SerializationMap& context)
{
	auto array = reader->ReadArray("components");
	for (SizeType i = 0; i < array->GetArraySize(); i++)
	{
		m_pComponentManager->DeserializeAndAddComponent(entity, reader, context);
	}
}

std::vector<std::shared_ptr<System>> Coordinator::ExtractSystems(std::shared_ptr<Coordinator> pOther)
{
	std::vector<std::shared_ptr<System>> toReturn;

	for (std::pair<const char*, std::shared_ptr<System>> const& pair : pOther->m_pSystemManager->m_Systems)
	{
		if (m_pSystemManager->m_Systems.find(pair.first) == m_pSystemManager->m_Systems.end())
		{
			std::shared_ptr<System> pSystem = pOther->m_pSystemManager->m_Systems.at(pair.first)->Clone();
			toReturn.push_back(pSystem);
			m_pSystemManager->m_Systems.insert(std::make_pair(pair.first, pSystem));

			Signature signature = pOther->m_pSystemManager->m_Signatures.at(pair.first);
			m_pSystemManager->m_Signatures.insert(std::make_pair(pair.first, signature));

			std::vector<Entity> entities = m_pEntityManager->GetEntitiesWithSignature(signature);
			for (Entity entity : entities)
			{
				pSystem->m_Entities.insert(entity);
			}
		}
	}

	return toReturn;
}

void Coordinator::SetEntityActive(Entity entity, bool isActive)
{
	auto components = m_pComponentManager->GetComponentsForSignature(entity, m_pEntityManager->GetSignature(entity));

	for (std::shared_ptr<ECS_Component> comp : components)
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

std::unordered_set<Entity> const& Coordinator::GetChildren(Entity entity)
{
	return m_pEntityManager->GetChildren(entity);
}

std::vector<std::shared_ptr<ECS_Component>> Coordinator::GetComponents(Entity entity)
{
	return m_pComponentManager->GetComponentsForSignature(entity, m_pEntityManager->GetSignature(entity));
}