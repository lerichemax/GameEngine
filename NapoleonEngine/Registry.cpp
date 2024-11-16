#include "PCH.h"
#include "Registry.h"

Registry::Registry()
	:m_pComponentManager{std::make_unique<ComponentManager>()},
	m_pEntityManager{ std::make_unique<EntityManager>() },
	m_pSystemManager{ std::make_unique<SystemManager>() }
{
}

Entity Registry::CreateEntity()
{
	return m_pEntityManager->CreateEntity();
}

void Registry::DestroyEntity(Entity entity)
{
	m_pEntityManager->DestroyEntity(entity);
	m_pComponentManager->EntityDestroyed(entity);
}

void Registry::TransferTags(Entity originEntity, Entity destinationEntity, Registry* const pOther)
{
	if (pOther->m_pEntityManager->EntityHasATag(originEntity))
	{
		m_pEntityManager->SetTag(destinationEntity, pOther->GetTag(originEntity));
	}
}

void Registry::SerializeEntities(StreamWriter& writer)
{
	writer.StartArray("entities");
	{
		for (Entity entity : m_pEntityManager->m_CreatedEntities)
		{
			writer.m_BufferWriter.StartObject();
			{
				writer.WriteInt(std::string{ "Entity" }, entity);
				writer.WriteString(std::string{ "Tag" }, GetTag(entity));

				writer.StartArray(std::string{ "components" });
				{
					auto components = GetEntityComponents(entity);

					for (Component* const pComp : components)
					{
						writer.m_BufferWriter.StartObject();
						writer.WriteString(std::string{ "type" }, typeid(*pComp).name());
						pComp->Serialize(writer);
						writer.m_BufferWriter.EndObject();
					}
				}
				writer.EndArray();

				auto children = GetChildren(entity);
				writer.StartArray(std::string{ "children" });
				for (Entity child : children)
				{
					writer.WriteIntNoKey(static_cast<int>(child));
				}
				writer.EndArray();
			}
			writer.m_BufferWriter.EndObject();
		}
	}
	writer.EndArray();
}

void Registry::DeserializeEntities(JsonReader const* reader, SerializationMap& context)
{
	auto objects = reader->ReadArray("entities");

	for (SizeType i = 0; i < objects->GetArraySize(); i++)
	{
		Entity newEntity = m_pEntityManager->CreateEntity();
		int entity;
		auto serializedEntityReader = objects->ReadArrayIndex(i);

		if (m_pEntityManager->m_CreatedEntities.size() > 5)
		{
			Entity* entt = &m_pEntityManager->m_CreatedEntities[5];
			entt = nullptr;
		}

		serializedEntityReader->ReadInt( "Entity" , entity);
		context.Add(entity, newEntity);

		DeserializeComponents(newEntity, serializedEntityReader->ReadArray( "components" ).get(), context);

		std::string tag;
		serializedEntityReader->ReadString( "Tag" , tag);
		SetTag(newEntity, tag);
	}
}

void Registry::RestoreEntitiesContext(JsonReader const* reader, SerializationMap const& context)
{
	auto entitiesObject = reader->ReadArray("entities");

	size_t arraySize = entitiesObject->GetArraySize();

	for (size_t i = 0; i < arraySize; i++)
	{
		Entity entity = *(m_pEntityManager->m_CreatedEntities.begin() + (m_pEntityManager->m_CreatedEntities.size() - (arraySize - i)));

		auto entityReadObject = entitiesObject->ReadArrayIndex(i);

		auto jsonComponents = entityReadObject->ReadArray( "components" );
		auto components = GetEntityComponents(entity);

		for (size_t j = 0; j < components.size(); j++)
		{
			components[j]->RestoreContext(jsonComponents->ReadArrayIndex(j).get(), context);
			components[j]->m_Entity = entity;
		}

		auto children = entityReadObject->ReadArray( "children" );
		if (children != nullptr && children->IsValid())
		{
			for (SizeType j = 0; j < children->GetArraySize(); j++)
			{
				int child = children->ReadArrayIndexAsInt(j);
				AddChild(entity, *context.GetRef<Entity>(child));
			}
		}
	}
}

void Registry::DeserializeComponents(Entity entity, JsonReader const* reader /*array*/, SerializationMap& context)
{
	Signature signature = m_pEntityManager->GetSignature(entity);
	for (SizeType i = 0; i < reader->GetArraySize(); i++)
	{
		auto arrayIndex = reader->ReadArrayIndex(i);
		ComponentType type = m_pComponentManager->DeserializeAndAddComponent(entity, arrayIndex.get(), context);
		signature.set(type, true);
	}
	
	m_pEntityManager->SetSignature(entity, signature);
}

void Registry::SetEntityActive(Entity entity, bool isActive)
{
	auto components = m_pComponentManager->GetComponentsForSignature(entity, m_pEntityManager->GetSignature(entity));

	for (Component* const comp : components)
	{
		comp->SetActive(isActive);
	}
}

void Registry::SetEntityHierarchyActive(Entity entity, bool isActive)
{
	SetEntityActive(entity, isActive);

	std::unordered_set<Entity> const& children = m_pEntityManager->GetChildren(entity);

	for (Entity child : children)
	{
		SetEntityActive(child, isActive);
	}
}

void Registry::AddChild(Entity parent, Entity child)
{
	m_pEntityManager->AddChild(parent, child);
}

void Registry::SetTag(Entity entity, std::string const& tag)
{
	m_pEntityManager->SetTag(entity, tag);
}

std::string Registry::GetTag(Entity entity) const
{
	return m_pEntityManager->GetTag(entity);
}

bool Registry::EntityHasTag(Entity entity, std::string const& tag) const
{
	return m_pEntityManager->EntityHasTag(entity, tag);
}

int Registry::GetLivingEntitiesCount() const
{
	return m_pEntityManager->m_LivingEntitiesCount;
}

Entity Registry::GetEntityAtIndex(int idx) const
{
	assert(idx < m_pEntityManager->m_LivingEntitiesCount && "idx out of range");
	return m_pEntityManager->m_CreatedEntities[idx];
}

std::unordered_set<Entity> const& Registry::GetChildren(Entity entity) const
{
	return m_pEntityManager->GetChildren(entity);
}

std::vector<Entity> Registry::GetEntityHierarchy(Entity entity) const
{
	return m_pEntityManager->GetEntityHierarchy(entity);
}

std::vector<Entity> Registry::GetEntitiesWithSignature(Signature const& signature)
{
	return m_pEntityManager->GetEntitiesWithSignature(signature);
}

std::vector<Component*> Registry::GetEntityComponents(Entity entity)
{
	return m_pComponentManager->GetComponentsForSignature(entity, m_pEntityManager->GetSignature(entity));
}