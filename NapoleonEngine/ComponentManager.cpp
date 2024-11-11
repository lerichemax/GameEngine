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

ComponentType ComponentManager::DeserializeAndAddComponent(Entity entity, JsonReader const* reader, SerializationMap& context)
{
	std::string type;
	reader->ReadString("type", type);

	auto pComp = Factory<Component, ComponentManager* const>::Get().Create(type, this);

	assert(pComp != nullptr && "Failed to construct object from type");

	pComp->Deserialize(reader, context);
	context.Add(pComp->GetId(), pComp);

	m_ComponentArrays.at(type)->ForceInsertData(pComp, entity);

	return m_ComponentTypes.at(type);
}