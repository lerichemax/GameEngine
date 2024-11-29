#pragma once
#include "Singleton.h"
#include "JsonReaderWriter.h"

#include <unordered_map>
#include <functional>
#include <string>

class IContextSerializable;
class SerializationMap;

namespace ecs 
{
	struct Component;

	template<typename T>
	concept ComponentDerived = std::derived_from<T, Component>;
}

template <ecs::ComponentDerived C>
C* CreateComponent();

class Command;

struct IPropertyInfo
{
public:
	virtual ~IPropertyInfo() = default;
	virtual void SerializeProperty(void* pContainingClass, StreamWriter& writer) = 0;
	virtual void DeserializeProperty(void* pContainingClass, JsonReader const* reader) = 0;
	virtual void RestorePropertyContext(void* pContainingClass, JsonReader const* reader, SerializationMap const& context) = 0;
};

template<typename PropType>
struct PropertyInfo : public IPropertyInfo //template specialize ?
{
public:
	std::string Name;
	size_t Offset;
	bool bIsRef;

	void SerializeProperty(void* pContainingClass, StreamWriter& writer) override;
	void DeserializeProperty(void* pContainingClass, JsonReader const* reader) override;
	void RestorePropertyContext(void* pContainingClass, JsonReader const* reader, SerializationMap const& context) override;
};

class IClassInfo // befriend reflection
{
public:
	virtual ~IClassInfo() = default;
	virtual void Serialize(void* const pClass, StreamWriter& writer) const = 0;
	virtual void Deserialize(void* pClass, JsonReader* const reader) = 0;
	virtual void RestoreContext(void* pClass, JsonReader const* reader, SerializationMap const& context) = 0;

	IClassInfo* pBaseClass = nullptr;
};

template <typename T>
class ClassInfo : public IClassInfo // befriend reflection
{
public:
	std::string Name;
	std::unordered_map<std::string, std::unique_ptr<IPropertyInfo>> Properties;

	void Serialize(void* const pClass, StreamWriter& writer) const override;
	void Deserialize(void* pClass, JsonReader* const reader) override;
	void RestoreContext(void* pClass, JsonReader const* reader, SerializationMap const& context) override;
};

class Reflection final : public Singleton<Reflection>
{
	friend class Singleton<Reflection>;

public:
	template <typename T, typename B = void> void RegisterClass();
	template <typename T, typename PropType> void RegisterProperty(T* const pObj, std::string const& propertyName, PropType T::* prop);
	template <typename T> void SerializeClass(T* pClass, StreamWriter& writer) const;
	template <typename T> void DeserializeClass(T* pClass, JsonReader* const reader) const;
	template <typename T> void RestoreClassContext(T* pClass, JsonReader* const reader, SerializationMap const& context) const;

private:
	Reflection() = default;

	template <typename T> ClassInfo<T>* GetClassInfo() const;

	std::unordered_map<std::string, std::unique_ptr<IClassInfo>> m_ClassRegistry;
};

struct AutoPropertyRegister {
	template<typename T, typename PropType>
	AutoPropertyRegister(T* const pObj, PropType T::* prop, const char* propertyName) //make this work with non pointers
	{
		Reflection::Get().RegisterProperty<T>(pObj, propertyName, prop);
	}
};

// Helper to count arguments
#define GET_MACRO(_1, _2, NAME, ...) NAME

// Default version when B is not provided
#define SERIALIZE_BASE_CLASS(T) \
    static struct T##AutoRegister { \
        T##AutoRegister() { Reflection::Get().RegisterClass<T>(); } \
    } T##_auto_register_instance;

// Version when both T and B are provided
#define SERIALIZE_DERIVED_CLASS(T, B) \
    static struct T##AutoRegister { \
        T##AutoRegister() { Reflection::Get().RegisterClass<T, B>(); } \
    } T##_auto_register_instance;

// Wrapper macro to choose the correct one
#define SERIALIZE_CLASS(...) GET_MACRO(__VA_ARGS__, SERIALIZE_DERIVED_CLASS, SERIALIZE_BASE_CLASS)(__VA_ARGS__)

#define PROPERTY(type, name) \
	type name; \
	AutoPropertyRegister const _autoPropertyRegister_##name{this, &std::decay_t<decltype(*this)>::name, #name}

#include "Reflection.inl"