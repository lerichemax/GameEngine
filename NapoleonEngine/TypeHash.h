#pragma once
// https://www.youtube.com/watch?v=LdVEq8fQ4DI&list=PL-wfxf45KxnRWBmzmuQ8I9Hsq6KbqW6UB&ab_channel=ConnorDeMeyer

#include <string_view>

namespace ReflectionSystem
{
	template <typename T> constexpr std::string_view TypeName();

	template<> 
	constexpr std::string_view TypeName<void>()
	{
		return "void";
	}

	using type_name_prober = void;

	template <typename T>
	constexpr std::string_view wrapped_type_name()
	{
		return __FUNCSIG__;
	}

	constexpr std::size_t wrapped_type_name_prefix_length()
	{
		return wrapped_type_name<type_name_prober>().find(TypeName<type_name_prober>());
	}

	constexpr std::size_t wrapped_type_name_suffix_length()
	{
		return wrapped_type_name<type_name_prober>().length() - wrapped_type_name_prefix_length() - TypeName<type_name_prober>().length();
	}

	template <typename T>
	constexpr std::string_view TypeName()
	{
		constexpr auto wrapped_name = wrapped_type_name<T>();
		constexpr auto prefix_length = wrapped_type_name_prefix_length();
		constexpr auto suffix_length = wrapped_type_name_suffix_length();
		constexpr auto type_name_length = wrapped_name.length() - prefix_length - suffix_length;
		return wrapped_name.substr(prefix_length, type_name_length);
	}

	// HASHING
	//FNV-1a hash algorythm

	constexpr uint64_t hash(std::string_view str)
	{
		constexpr uint64_t FNV_offset_basis = 0xCBF29CE484222325ULL;
		constexpr uint64_t FNV_prime = 0x100000001B3ULL;
		uint64_t hash = FNV_offset_basis;
		for (char c : str)
		{
			hash ^= static_cast<uint64_t>(c);
			hash *= FNV_prime;
		}
		return hash;
	}

	//TYPE ID
	template <typename Type>
	constexpr uint64_t TypeHash()
	{
		return hash(TypeName<Type>());
	}
}