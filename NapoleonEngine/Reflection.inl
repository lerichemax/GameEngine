#include "Reflection.h"
#include "SerializationMap.h"

#include <concepts>

template<typename T>
struct is_smart_pointer : std::false_type {};

// Specialization for std::unique_ptr
template<typename T>
struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};

// Specialization for std::shared_ptr
template<typename T>
struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

template<typename PropType>
void PropertyInfo<PropType>::SerializeProperty(void* const pContainingClass, StreamWriter& writer)
{
	char* pClassBytes = static_cast<char*>(pContainingClass);
	pClassBytes += Offset;

	PropType* pProp = reinterpret_cast<PropType*>(pClassBytes);

	if constexpr (is_smart_pointer<PropType>::value)
	{
		writer.Write(Name, (*pProp).get());
	}
	else
	{
		writer.Write(Name, *pProp);
	}
}

template<typename PropType>
void PropertyInfo<PropType>::DeserializeProperty(void* pContainingClass, JsonReader const* reader, SerializationMap& context)
{
	char* pClassBytes = static_cast<char*>(pContainingClass);
	pClassBytes += Offset;

	PropType* pProp = reinterpret_cast<PropType*>(pClassBytes);

	if constexpr  (std::is_base_of<IContextSerializable, PropType>::value)
	{
		context.Add(pProp->GetId(), pProp);
	}

	if constexpr (is_smart_pointer<PropType>::value)
	{
		reader->Read(Name, (*pProp).get());
	}
	else
	{
		reader->Read(Name, *pProp);
	}
}
template<typename PropType>
void PropertyInfo<PropType>::RestorePropertyContext(void* pContainingClass, JsonReader const* reader, SerializationMap const& context)
{
	if (!bIsRef || Name != "m_Entity")
	{
		return;
	}

	char* pClassBytes = static_cast<char*>(pContainingClass);
	pClassBytes += Offset;

	PropType* pProp = reinterpret_cast<PropType*>(pClassBytes);

	int id = -1;
	reader->Read(Name, id);

	pProp = context.GetRef<PropType>(id);
}

template <typename T>
void Reflection::RegisterClass()
{
	std::string const typeName = typeid(T).name();

	auto classIt = m_ClassRegistry.find(typeName);

	if (classIt != m_ClassRegistry.end())
	{
		//class already registered
		return;
	}

	std::unique_ptr<ClassInfo> pInfo = std::make_unique<ClassInfo>();
	pInfo->Name = typeName;
	if constexpr (std::is_base_of<ecs::Component, T>::value)
	{
		Factory<ecs::Component>::Get().RegisterType<T>(CreateComponent<T>);
	}

	m_ClassRegistry[typeName] = std::move(pInfo);
}

template <typename T, typename PropType>
void Reflection::RegisterProperty(std::string const& propertyName, PropType T::* prop)
{
	std::string const typeName = typeid(T).name();
	ClassInfo* const pInfo = m_ClassRegistry[typeName].get();

	auto propIt = pInfo->Properties.find(propertyName);

	if (propIt != pInfo->Properties.end())
	{
		//Property already registered
		return;
	}

	size_t offset = reinterpret_cast<size_t>(&(reinterpret_cast<T*>(nullptr)->*prop)); //get the byte offset of prop within the object

	std::unique_ptr<PropertyInfo<PropType>> pProp = std::make_unique<PropertyInfo<PropType>>();
	pProp->Name = propertyName;
	pProp->Offset = offset;
	pProp->bIsRef = std::is_base_of<IContextSerializable, PropType>::value;

	pInfo->Properties[propertyName] = std::move(pProp);
}

template <typename T>
void Reflection::SerializeClass(T* const pClass, StreamWriter& writer) const
{
	std::string const typeName = typeid(*pClass).name();

	writer.m_BufferWriter.StartObject();
	writer.WriteString("type", typeName);

	for (auto const& pair : m_ClassRegistry.at(typeName)->Properties)
	{
		pair.second->SerializeProperty(pClass, writer);
	}

	writer.m_BufferWriter.EndObject();
}

template <typename T> 
void Reflection::DeserializeClass(T* pClass, JsonReader* const reader, SerializationMap& context) const
{
	std::string const typeName = typeid(*pClass).name();

	for (auto const& pair : m_ClassRegistry.at(typeName)->Properties)
	{
		pair.second->DeserializeProperty(pClass, reader, context);
	}
}

template <typename T> 
void Reflection::RestoreClassContext(T* pClass, JsonReader* const reader, SerializationMap const& context) const
{
	std::string const typeName = typeid(*pClass).name();

	for (auto const& pair : m_ClassRegistry.at(typeName)->Properties)
	{
		pair.second->RestorePropertyContext(pClass, reader, context);
	}
}