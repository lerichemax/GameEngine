#include "PCH.h"
#include "ComponentManager.h"
#include "TransformComponent.h"
#include "ButtonComponent.h"
#include "RendererComponent.h"
#include "TextRendererComponent.h"
#include "CameraComponent.h"
#include "FPSCounter.h"

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
		if (signature[pair.second] == 1)
		{
			components.push_back(m_ComponentArrays.at(pair.first)->GetBaseData(entity));
		}
	}

	return components;
}

ComponentType ComponentManager::DeserializeAndAddComponent(Entity entity, JsonReader const* reader, SerializationMap& context)
{
	std::string type;
	reader->ReadString("type", type);

	ForceRegisterComponent(type);

	auto pComp = GetComponent(type);
	pComp->Deserialize(reader, context);

	m_ComponentArrays.at(type.c_str())->ForceInsertData(pComp, entity);

	return m_ComponentTypes.at(type);
}

std::shared_ptr<ECS_Component> ComponentManager::GetComponent(std::string const& type)
{
	if (type == "class ECS_TransformComponent")
	{
		return std::static_pointer_cast<ECS_Component>(std::make_shared<ECS_TransformComponent>());
	}
	else if (type == "class ECS_ButtonComponent")
	{
		return std::static_pointer_cast<ECS_Component>(std::make_shared<ECS_ButtonComponent>());
	}
	else if (type == "struct ECS_CameraComponent")
	{
		return std::static_pointer_cast<ECS_Component>(std::make_shared<ECS_CameraComponent>());
	}
	else if (type == "struct ECS_RendererComponent")
	{
		return std::static_pointer_cast<ECS_Component>(std::make_shared<ECS_RendererComponent>());
	}
	else if (type == "struct ECS_TextRendererComponent")
	{
		return std::static_pointer_cast<ECS_Component>(std::make_shared<ECS_TextRendererComponent>());
	}
	else if (type == "struct FPSCounterComponent")
	{
		return std::static_pointer_cast<ECS_Component>(std::make_shared<FPSCounterComponent>());
	}
	return nullptr;
}

void ComponentManager::ForceRegisterComponent(std::string const& type)
{
	if (type == "class ECS_TransformComponent")
	{
		RegisterComponent<ECS_TransformComponent>();
	}
	else if (type == "class ECS_ButtonComponent")
	{
		RegisterComponent<ECS_ButtonComponent>();
	}
	else if (type == "struct ECS_CameraComponent")
	{
		RegisterComponent<ECS_CameraComponent>();
	}
	else if (type == "struct ECS_RendererComponent")
	{
		RegisterComponent<ECS_RendererComponent>();
	}
	else if (type == "struct ECS_TextRendererComponent")
	{
		RegisterComponent<ECS_TextRendererComponent>();
	}
	else if (type == "struct FPSCounterComponent")
	{
		RegisterComponent<FPSCounterComponent>();
	}
}