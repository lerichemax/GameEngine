#include "PCH.h"
#include "ComponentManager.h"

void ComponentManager::EntityDestroyed(Entity entity)
{
	for (std::pair<const char*, std::shared_ptr<IComponentArray>> const& pair : m_ComponentArrays)
	{
		std::shared_ptr<IComponentArray> pComponent = pair.second;

		pComponent->EntityDestroyed(entity);
	}
}