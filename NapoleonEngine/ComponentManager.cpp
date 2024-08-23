#include "PCH.h"
#include "ComponentManager.h"
#include "Factories.h"

#include "RendererComponent.h"

std::unordered_map<std::string, ComponentType> ComponentManager::m_ComponentTypes = std::unordered_map<std::string, ComponentType>{};
ComponentType ComponentManager::m_NextComponentType = ComponentType{};

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
		if (pair.second == GetComponentType<BehaviourComponent>() || pair.second == GetComponentType<RendererComponent>())
		{
			continue;
		}

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

	auto pComp = Factory<Component, ComponentManager* const>::GetInstance().Create(type, this);

	assert(pComp != nullptr && "Failed to construct object from type");

	pComp->Deserialize(reader, context);
	context.Add(pComp->GetId(), pComp);

	m_ComponentArrays.at(type.c_str())->ForceInsertData(pComp, entity);

	//bool isException{ false };
	//reader->ReadBool("behaviour", isException);
	//if (isException)
	//{
	//	m_ComponentArrays.at(typeid(BehaviourComponent).name())->ForceInsertData(pCompShared, entity);
	//}
	//reader->ReadBool("renderer", isException);
	//if (isException)
	//{
	//	m_ComponentArrays.at(typeid(RendererComponent).name())->ForceInsertData(pCompShared, entity);
	//}

	return m_ComponentTypes.at(type);
}