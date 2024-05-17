#include "PCH.h"
#include "ComponentManager.h"

std::unordered_map<const char*, ComponentType> ComponentManager::m_ComponentTypes = std::unordered_map<const char*, ComponentType>{};
ComponentType ComponentManager::m_NextComponentType = ComponentType{};

void ComponentManager::EntityDestroyed(Entity entity)
{
	for (std::pair<const char*, std::shared_ptr<IComponentArray>> const& pair : m_ComponentArrays)
	{
		std::shared_ptr<IComponentArray> pComponent = pair.second;

		pComponent->EntityDestroyed(entity);
	}
}

ComponentManager::~ComponentManager()
{

}