#include "PCH.h"
#include "ComponentManager.h"
#include "Factories.h"


std::unordered_map<size_t, ComponentType> ComponentManager::m_ComponentTypes = std::unordered_map<size_t, ComponentType>{};
ComponentType ComponentManager::m_NextComponentType = ComponentType{};

void ComponentManager::CleanUp()
{
	m_ComponentTypes.clear();
	std::unordered_map<size_t, ComponentType>().swap(m_ComponentTypes);
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

	for (std::pair<size_t, ComponentType> const& pair : m_ComponentTypes)
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
	int64_t typeInt;
	reader->ReadInt64("type", typeInt);

	size_t hash = static_cast<size_t>(typeInt);

	auto pComp = Factory<Component, ComponentManager* const>::Get().Create(hash, this);

	assert(pComp != nullptr && "Failed to construct object from type");

	pComp->Deserialize(reader, context);
	context.Add(pComp->GetId(), pComp);

	m_ComponentArrays.at(hash)->ForceInsertData(pComp, entity);

	return m_ComponentTypes.at(hash);
}