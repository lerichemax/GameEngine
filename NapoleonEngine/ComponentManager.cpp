#include "PCH.h"
#include "ComponentManager.h"

std::unordered_map<const char*, ComponentType> ComponentManager::m_ComponentTypes = std::unordered_map<const char*, ComponentType>{};
ComponentType ComponentManager::m_NextComponentType = ComponentType{};

ComponentManager::~ComponentManager()
{
	m_ComponentTypes.clear(); //TODO -> destroy in coordinator
}

void ComponentManager::EntityDestroyed(Entity entity)
{
	for (std::pair<const char*, std::shared_ptr<IComponentArray>> const& pair : m_ComponentArrays)
	{
		std::shared_ptr<IComponentArray> pComponent = pair.second;

		pComponent->EntityDestroyed(entity);
	}
}

std::vector<std::shared_ptr<ECS_Component>> ComponentManager::GetComponentsForSignature(Entity entity, Signature signature)
{
	std::vector<std::shared_ptr<ECS_Component>> components;

	for (std::pair<const char*, ComponentType> const& pair : m_ComponentTypes)
	{
		if (signature[pair.second] == 1)
		{
			components.push_back(m_ComponentArrays.at(pair.first)->GetBaseData(entity));
		}
	}

	return components;
}

void ComponentManager::DeserializeAndAddComponent(Entity entity, JsonReader const* reader)
{
	std::string type;
	reader->ReadString("type", type);

	if (m_ComponentTypes.find(type.c_str()) == m_ComponentTypes.end())
	{
		m_ComponentTypes.insert(std::make_pair(type.c_str(), m_NextComponentType++));
	}

	auto pComp = GetComponent(type);
	pComp->Deserialize(reader);

	m_ComponentArrays.at(type.c_str())->ForceInsertData(pComp, entity);
}

std::shared_ptr<ECS_Component> ComponentManager::GetComponent(std::string const& type)
{
	if (type == "class ECS_TransformComponent")
	{
		return std::static_pointer_cast<ECS_Component>(std::make_shared<ECS_TransformComponent>());
	}
}