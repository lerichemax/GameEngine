#pragma once
#include "Component.h"
#include "ComponentArray.h"
#include "Entity.h"
#include "JsonReaderWriter.h"
#include "Helpers.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <typeindex>

namespace ecs
{
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
		template<ComponentDerived T> std::vector<T*> const FindComponentsOfType();
		template<ComponentDerived T> void RegisterComponent();

		void EntityDestroyed(Entity entity);
		std::vector<Component*> GetComponentsForSignature(Entity entity, Signature signature);

	private:
		friend class Registry;

		static std::unordered_map<std::string, ComponentType> m_ComponentTypes;
		std::unordered_map<std::string, std::unique_ptr<IComponentArray>> m_ComponentArrays{};
		static ComponentType m_NextComponentType;

		template<ComponentDerived T> ComponentArray<T>* const GetComponentArray();
		template<ComponentDerived T> void RegisterComponentArray(std::string const& type);

		ComponentType DeserializeAndAddComponent(Entity entity, JsonReader* const reader, SerializationMap& context);
		void RestoreEntityComponentContext(Entity entity, JsonReader* const reader, SerializationMap const& context);

		static void CleanUp();
	};
}

#include "ComponentManager.inl"