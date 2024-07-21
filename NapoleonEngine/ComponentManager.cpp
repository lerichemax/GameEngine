#include "PCH.h"
#include "ComponentManager.h"
#include "Factories.h"

std::unordered_map<std::string, ComponentType> ComponentManager::m_ComponentTypes = std::unordered_map<std::string, ComponentType>{};
ComponentType ComponentManager::m_NextComponentType = ComponentType{};

void ComponentManager::EntityDestroyed(Entity entity)
{
	for (std::pair<std::string, std::shared_ptr<IComponentArray>> const& pair : m_ComponentArrays)
	{
		std::shared_ptr<IComponentArray> pComponent = pair.second;

		pComponent->EntityDestroyed(entity);
	}
}

std::vector<std::shared_ptr<ECS_Component>> ComponentManager::GetComponentsForSignature(Entity entity, Signature signature)
{
	std::vector<std::shared_ptr<ECS_Component>> components;

	for (std::pair<std::string, ComponentType> const& pair : m_ComponentTypes)
	{
		if (pair.second == GetComponentType<BehaviourComponent>())
		{
			continue;
		}

		if (signature[pair.second] == 1)
		{
			std::vector<std::shared_ptr<ECS_Component>> datas;
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

	auto pComp = ComponentFactory::GetInstance().Create(type, this);

	assert(pComp != nullptr && "Failed to construct object from type");

	pComp->Deserialize(reader, context);
	context.Add(pComp->GetId(), pComp);

	m_ComponentArrays.at(type.c_str())->ForceInsertData(pComp, entity);

	bool isBehaviour{ false };
	reader->ReadBool("behaviour", isBehaviour);
	if (isBehaviour)
	{
		m_ComponentArrays.at(typeid(BehaviourComponent).name())->ForceInsertData(pComp, entity);
	}


	return m_ComponentTypes.at(type);
}