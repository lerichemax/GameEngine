#pragma once
#include "Component.h"
#include "ComponentArray.h"
#include "Entity.h"
#include "JsonReaderWriter.h"
#include "Factories.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <typeindex>

class ComponentManager final
{
	friend class NapoleonEngine;
public:
	~ComponentManager() = default;

	template<ComponentDerived T> ComponentType GetComponentType();

	template<ComponentDerived T> T* const AddComponent(Entity entity);

	template<ComponentDerived T> void RemoveComponent(Entity entity);
	template<ComponentDerived T> T* const GetComponent(Entity entity);
	template<ComponentDerived T> std::vector<T*> GetComponents(Entity entity);
	template<ComponentDerived T> T* const FindComponentOfType();
	
	void EntityDestroyed(Entity entity);
	std::vector<Component*> GetComponentsForSignature(Entity entity, Signature signature);

private:
	friend class Coordinator;

	static std::unordered_map<size_t, ComponentType> m_ComponentTypes;
	std::unordered_map<size_t, std::unique_ptr<IComponentArray>> m_ComponentArrays{};
	static ComponentType m_NextComponentType;

	template<ComponentDerived T> void RegisterComponent();
	template<ComponentDerived T> ComponentArray<T>* const GetComponentArray();
	template<ComponentDerived T> void RegisterComponentArray(size_t typeHash);

	ComponentType DeserializeAndAddComponent(Entity entity, JsonReader const* reader, SerializationMap& context);
	static void CleanUp();
};

#include "ComponentManager.inl"