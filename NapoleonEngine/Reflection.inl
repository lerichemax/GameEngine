#include "Reflection.h"
#include "SerializationMap.h"

#include "Factories.h"

#include <concepts>
#include <cstddef>

template<typename T>
struct is_smart_pointer : std::false_type {};

// Specialization for std::unique_ptr
template<typename T>
struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};

// Specialization for std::shared_ptr
template<typename T>
struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

//Property Info

template<typename PropType>
void PropertyInfo<PropType>::SerializeProperty(void* const pContainingClass, StreamWriter& writer)
{
	char* pClassBytes = reinterpret_cast<char*>(pContainingClass);

	pClassBytes += Offset;

	PropType* pProp = reinterpret_cast<PropType*>(pClassBytes);

	if constexpr (is_smart_pointer<PropType>::value)
	{
		
		if constexpr (std::is_base_of<ecs::Component, std::remove_pointer_t<PropType>>::value)
		{
			writer.Write(Name, (*pProp).get()->GetId());
		}
		else 
		{
			writer.Write(Name, (*pProp).get());
		}
	}
	else
	{
		if constexpr (std::is_base_of<ecs::Component, std::remove_pointer_t<PropType>>::value)
		{
			if ((*pProp) != nullptr)
			{
				writer.Write(Name, (*pProp)->GetId());
			}
		}
		else
		{
			writer.Write(Name, *pProp);
		}
	}
}

template<typename PropType>
void PropertyInfo<PropType>::DeserializeProperty(void* pContainingClass, JsonReader const* reader)
{
	char* pClassBytes = static_cast<char*>(pContainingClass);
	pClassBytes += Offset;

	PropType* pProp = reinterpret_cast<PropType*>(pClassBytes);

	if constexpr (is_smart_pointer<PropType>::value)
	{
		if (!(*pProp))
		{
			*pProp = std::unique_ptr<std::remove_pointer_t<decltype((*pProp).get())>>(static_cast<decltype((*pProp).get())>(nullptr));

			auto pPtr = static_cast<decltype((*pProp).get())>(nullptr);
			if (!bIsRef)
			{
				reader->Read(Name, pPtr);
				(*pProp).reset(pPtr);
			}

		}
	}
	else
	{
		if (!bIsRef)
		{
			reader->Read(Name, *pProp);
		}
	}
}
template<typename PropType>
void PropertyInfo<PropType>::RestorePropertyContext(void* pContainingClass, JsonReader const* reader, SerializationMap const& context)
{
	if (!bIsRef)
	{
		return;
	}

	char* pClassBytes = static_cast<char*>(pContainingClass);
	pClassBytes += Offset;

	PropType* pProp = reinterpret_cast<PropType*>(pClassBytes);

	int id = -1;
	reader->Read(Name, id);

	if constexpr (std::is_base_of<ecs::Component, std::remove_pointer_t<PropType>>::value)
	{
		*pProp = context.GetRef<std::remove_pointer_t<PropType>>(id);
	}
}

//Class Info

template <typename T>
void ClassInfo<T>::Serialize(void* const pClass, StreamWriter& writer) const
{
	for (auto const& pair : Properties)
	{
		pair.second->SerializeProperty(pClass, writer);
	}

	if (pBaseClass != nullptr)
	{
		pBaseClass->Serialize(pClass, writer);
	}
}

template <typename T>
void ClassInfo<T>::Deserialize(void* pClass, JsonReader* const reader)
{
	for (auto const& pair : Properties)
	{
		pair.second->DeserializeProperty(pClass, reader);
	}

	if (pBaseClass != nullptr)
	{
		pBaseClass->Deserialize(pClass, reader);
	}
}

template <typename T>
void ClassInfo<T>::RestoreContext(void* pClass, JsonReader const* reader, SerializationMap const& context)
{
	for (auto const& pair : Properties)
	{
		pair.second->RestorePropertyContext(pClass, reader, context);
	}

	if (pBaseClass != nullptr)
	{
		pBaseClass->RestoreContext(pClass, reader, context);
	}
}

//Reflection

template <typename T, typename B>
void Reflection::RegisterClass()
{
	std::string const typeName = typeid(T).name();

	auto classIt = m_ClassRegistry.find(typeName);

	if (classIt != m_ClassRegistry.end())
	{
		//class already registered
		return;
	}

	std::unique_ptr<ClassInfo<T>> pInfo = std::make_unique<ClassInfo<T>>();
	pInfo->Name = typeName;

	if constexpr (std::is_base_of<ecs::Component, T>::value)
	{
		Factory::Get().RegisterType<T>(CreateComponent<T>);
	}
	else if constexpr (!std::is_abstract<T>::value)
	{
		Factory::Get().RegisterType<T>([]() { return new T{}; });
	}

	ClassInfo<B>* pBase = GetClassInfo<B>();
	if (pBase != nullptr)
	{
		if constexpr (std::is_base_of_v<B, T> && !std::is_same_v<T, B>)
		{
			pInfo->pBaseClass = pBase;
		}
	}

	m_ClassRegistry[typeName] = std::move(pInfo);
}

template <typename T, typename PropType>
void Reflection::RegisterProperty(T* const pObj, std::string const& propertyName, PropType T::* prop)
{
	std::string const typeName = typeid(T).name();
	auto pInfo = GetClassInfo<T>();

	auto propIt = pInfo->Properties.find(propertyName);

	if (propIt != pInfo->Properties.end())
	{
		//Property already registered
		return;
	}

	size_t offset = reinterpret_cast<size_t>(&(pObj->*prop)) - reinterpret_cast<size_t>(pObj); //get the byte offset of prop within the object

	std::unique_ptr<PropertyInfo<PropType>> pProp = std::make_unique<PropertyInfo<PropType>>();
	pProp->Name = propertyName;
	pProp->Offset = offset;
	pProp->bIsRef = std::is_base_of<ecs::Component, std::remove_pointer_t<PropType>>::value;

	pInfo->Properties[propertyName] = std::move(pProp);
}

template <typename T>
void Reflection::SerializeClass(T* const pClass, StreamWriter& writer) const
{
	std::string const typeName = typeid(*pClass).name();

	writer.m_BufferWriter.StartObject();
	writer.WriteString("type", typeName);

	m_ClassRegistry.at(typeName)->Serialize(pClass, writer);

	writer.m_BufferWriter.EndObject();
}

template <typename T> 
void Reflection::DeserializeClass(T* pClass, JsonReader* const reader) const
{
	std::string const typeName = typeid(*pClass).name();

	m_ClassRegistry.at(typeName)->Deserialize(pClass, reader);
}

template <typename T> 
void Reflection::RestoreClassContext(T* pClass, JsonReader* const reader, SerializationMap const& context) const
{
	std::string const typeName = typeid(*pClass).name();

	m_ClassRegistry.at(typeName)->RestoreContext(pClass, reader, context);
}

template <typename T> 
ClassInfo<T>* Reflection::GetClassInfo() const
{
	std::string const typeName = typeid(T).name();

	if (m_ClassRegistry.find(typeName) != m_ClassRegistry.end())
	{
		return static_cast<ClassInfo<T>*>(m_ClassRegistry.at(typeName).get());
	}

	return nullptr;
}