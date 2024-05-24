#include "PCH.h"
#include "ComponentManager.h"

std::unordered_map<const char*, ComponentType> ComponentManager::m_ComponentTypes = std::unordered_map<const char*, ComponentType>{};
ComponentType ComponentManager::m_NextComponentType = ComponentType{};

ComponentManager::~ComponentManager()
{
	m_ComponentTypes.clear();
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