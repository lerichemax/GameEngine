#include "PCH.h"
#include "ComponentManager.h"
#include "Factories.h"

std::unordered_map<std::string, ComponentType> ComponentManager::m_ComponentTypes = std::unordered_map<std::string, ComponentType>{};
ComponentType ComponentManager::m_NextComponentType = ComponentType{};

void ComponentManager::CleanUp()
{
	m_ComponentTypes.clear();
	std::unordered_map<std::string, ComponentType>().swap(m_ComponentTypes);
	m_NextComponentType = 0;
}

void ComponentManager::EntityDestroyed(Entity entity)
{
	for (auto const& pair : m_ComponentArrays)
	{
		if (pair.second != nullptr)
		{
			pair.second->EntityDestroyed(entity);
		}
	}
}

std::vector<Component*> ComponentManager::GetComponentsForSignature(Entity entity, Signature signature)
{
	std::vector<Component*> components;

	for (std::pair<std::string, ComponentType> const& pair : m_ComponentTypes)
	{
		if (signature[pair.second] == 1)
		{
			std::vector<Component*> datas;
			if (m_ComponentArrays.at(pair.first)->TryGetAllBaseData(entity, datas))
			{
				components.insert(components.end(), datas.begin(), datas.end());
			}
		}
	}

	return components;
}

ComponentType ComponentManager::DeserializeAndAddComponent(Entity entity, JsonReader* const reader, SerializationMap& context)
{
	std::string type;
	reader->ReadString("type", type);

	auto pComp = Factory::Get().Create<Component>(type);

	assert(pComp != nullptr && "Failed to construct object from type");

	Reflection::Get().DeserializeClass(pComp, reader);
	context.Add(pComp->GetId(), pComp);

	m_ComponentArrays.at(type)->ForceInsertData(pComp, entity);
	return m_ComponentTypes.at(type);
}

void ComponentManager::RestoreEntityComponentContext(Entity entity, JsonReader* const reader, SerializationMap const& context)
{
	std::string type;
	reader->ReadString("type", type);

	auto pComp = m_ComponentArrays.at(type)->GetBaseData(entity);
	if (IS_VALID(pComp))
	{
		Reflection::Get().RestoreClassContext(pComp, reader, context);
		pComp->m_Entity = entity;
	}
}