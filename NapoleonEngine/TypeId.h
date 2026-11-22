#pragma once
// https://www.youtube.com/watch?v=LdVEq8fQ4DI&list=PL-wfxf45KxnRWBmzmuQ8I9Hsq6KbqW6UB&ab_channel=ConnorDeMeyer

#include <unordered_map>
#include <cassert>
#include <bitset>

#include "TypeHash.h"

namespace ecs
{
	class Component;
}

enum TypeFlags
{
	TypeFlag_IsFundamental,
	TypeFlag_IsIntegral,
	TypeFlag_IsFloatingPoint,
	TypeFlag_IsTriviallyCopyable,

	TypeFlag_SIZE
};

struct TypeInfo final
{
	std::string_view Name{ };
	uint32_t Size{};
	uint32_t Alignment{ };

	std::bitset<TypeFlag_SIZE> Flags;

	bool bIsComponent{ false };

	std::is_function<ecs::Component*()> ComponentConstructor;

	template<typename T>
	static constexpr TypeInfo Create();
};

class TypeId final
{
public:
	constexpr TypeId() = default;
	constexpr TypeId(uint64_t hash) : ID(hash) {}

	template<typename T>
	static constexpr TypeId Create();

	constexpr void SetTypeId(uint64_t typeId) { ID = typeId; }
	constexpr uint64_t GetId() const { return ID; }

	TypeInfo const& GetTypeInfo() const { return GetTypeInformation(*this); }
	static TypeInfo const& GetTypeInformation(TypeId typeId)
	{
		auto& typeInfos = GetStatics();
		assert(typeInfos.TypeInfos.contains(typeId.GetId()));
		return typeInfos.TypeInfos[typeId.GetId()];
	}

	static auto const& GetAllTypeInformations()
	{
		return GetStatics();
	}

	template<typename T>
	static void RegisterTypeId()
	{
		uint64_t const typeId = TypeId<T>::Create().GetId();

		assert(!TypeInfos.contains(typeId) && "TypeId already registered");

		TypeInfos.emplace(typeId, TypeInfo::Create<T>());
	}

	friend bool operator==(TypeId const& lhs, TypeId const& rhs)
	{
		return lhs.ID == rhs.ID;
	}

private:
	struct StaticData
	{
		std::unordered_map<uint64_t, TypeInfo> TypeInfos{};
	};

	static StaticData& GetStatics()
	{
		static StaticData data{};
		return data;
	}

	int ID;
};

template<typename T>
inline constexpr TypeInfo TypeInfo::Create()
{
	TypeInfo info;
	info.Name = TypeName<T>();
	info.Size = sizeof(T);
	info.Alignment = alignof(T);

	info.Flags.set(TypeFlag_IsFundamental, std::is_fundamental<T>::value);
	info.Flags.set(TypeFlag_IsIntegral, std::is_integral<T>::value);
	info.Flags.set(TypeFlag_IsFloatingPoint, std::is_floating_point<T>::value);
	info.Flags.set(TypeFlag_IsTriviallyCopyable, std::is_trivially_copyable<T>::value);

	if (std::is_base_of<ecs::Component, T>::value)
	{
		info.bIsComponent = true;
		info.ComponentConstructor = []() -> ecs::Component* { CreateComponent<T>() }
	}

	return info;
}

template <typename T>
inline constexpr TypeId TypeId::Create()
{
	return TypeId(Reflection::TypeHash<T>());
}

template <typename T>
class RegisterType
{
private:
	class RegisterTypeOnce
	{
	public:
		RegisterTypeOnce()
		{
			TypeId::RegisterTypeId<T>();
		}
		inline static RegisterTypeOnce Registerer{};
	};
};

#define _REGISTER_TYPE_INTERNAL(TYPE, VARNAME) RegisterType<TYPE> VARNAME##TYPE {};
#define REGISTER_TYPE(TYPE) _REGISTER_TYPE_INTERNAL(TYPE, RegisterType_)
