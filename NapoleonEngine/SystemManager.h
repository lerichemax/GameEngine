#pragma once
#include "Component.h"
#include "Entity.h"
#include "System.h"

#include <unordered_map>
#include <memory>
#include <concepts>
#include <typeindex>

template <typename T>
concept SystemDerived = std::derived_from<T, System>;

class ComponentManager;
class Registry;
class SystemManager
{
public:
	template <SystemDerived T> T* const RegisterSystem(Registry* const pRegistry);
	template <SystemDerived T> T* const GetSystem() const;

private:
	friend class Registry;

	std::unordered_map<std::string, std::unique_ptr<System>> m_Systems;
};

#include "SystemManager.inl"