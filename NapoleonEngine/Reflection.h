#pragma once
#include "Singleton.h"
#include "Serializer.h"

#include <unordered_map>
#include <functional>
#include <string>

class IContextSerializable;
class SerializationMap;

namespace ecs 
{
	template<typename T>
	concept ComponentDerived = std::derived_from<T, Component>;
}

template <ecs::ComponentDerived C>
C* CreateComponent();

namespace ecs
{
	struct Component;
}

struct IPropertyInfo
{
public:
	virtual ~IPropertyInfo() = default;
	virtual void SerializeProperty(void* pContainingClass, StreamWriter& writer) = 0;
	virtual void DeserializeProperty(void* pContainingClass, JsonReader const* reader, SerializationMap& context) = 0;
	virtual void RestorePropertyContext(void* pContainingClass, JsonReader const* reader, SerializationMap const& context) = 0;
};

template<typename PropType>
struct PropertyInfo : public IPropertyInfo
{
public:
	std::string Name;
	size_t Offset;
	bool bIsRef;

	void SerializeProperty(void* pContainingClass, StreamWriter& writer) override;
	void DeserializeProperty(void* pContainingClass, JsonReader const* reader, SerializationMap& context) override;
	void RestorePropertyContext(void* pContainingClass, JsonReader const* reader, SerializationMap const& context) override;
};

struct ClassInfo // template ?
{
	std::string Name;
	std::function<ecs::Component* const()> CreateInstance;
	std::unordered_map<std::string, std::unique_ptr<IPropertyInfo>> Properties;
};

class Reflection final : public Singleton<Reflection>
{
	friend class Singleton<Reflection>;

public:
	template <typename T> void RegisterClass();
	template <typename T, typename PropType> void RegisterProperty(std::string const& propertyName, PropType T::* prop);
	template <typename T> void SerializeClass(T* pClass, StreamWriter& writer) const;
	template <typename T> void DeserializeClass(T* pClass, JsonReader* const reader, SerializationMap& context) const;
	template <typename T> void RestoreClassContext(T* pClass, JsonReader* const reader, SerializationMap const& context) const;
	
	ecs::Component* const CreateInstance(std::string const& typeName);

private:
	Reflection() = default;

	std::unordered_map<std::string, std::unique_ptr<ClassInfo>> m_ClassRegistry; // make ptr ?
};

struct AutoPropertyRegister {
	template<typename T, typename PropType>
	AutoPropertyRegister(T& obj, PropType T::* prop, const char* propertyName) //make this work with non pointers
	{
		Reflection::Get().RegisterProperty<T>(propertyName, prop);
	}
};

#define SERIALIZE_CLASS(T) \
	static struct T##AutoRegister { \
		T##AutoRegister() { Reflection::Get().RegisterClass<T>(); } \
	} T##_auto_register_instance;

#define PROPERTY(type, name) \
	type name; \
	AutoPropertyRegister const _autoPropertyRegister_##name{*this, &std::decay_t<decltype(*this)>::name, #name}

#include "Reflection.inl"