#pragma once

#include "TypeId.h"

//****************************************************
// https://stackoverflow.com/questions/9851594/standard-c11-way-to-remove-all-pointers-of-a-type
//****************************************************
template <typename T> class remove_all_pointers {
public:
	typedef T type;
}; 

template <typename T> class remove_all_pointers<T*> {
public:
	typedef typename remove_all_pointers<T>::type type;
};

template <typename T> class remove_all_pointers<T* const> {
public:
	typedef typename remove_all_pointers<T>::type type;
};

template <typename T> class remove_all_pointers<T* volatile> {
public:
	typedef typename remove_all_pointers<T>::type type;
};

template <typename T> class remove_all_pointers<T* const volatile > {
public:
	typedef typename remove_all_pointers<T>::type type;
};

template <typename T>
using remove_all_pointers_t = remove_all_pointers<T>::Type;

//****************************************************

class VariableId final
{
public:
	template <typename T>
	static constexpr VariableId Create();

	constexpr TypeId GetTypeId() const { return Type; }
	constexpr void SetTypeId(TypeId typeId) { Type = typeId; }

	constexpr void SetConstFlag() { TraitFlag |= ConstFlag; }
	constexpr void SetReferenceFlag() { TraitFlag |= ReferenceFlag; }
	constexpr void SetVolatileFlag() { TraitFlag |= VolatileFlag; }
	constexpr void SetRValReferenceFlag() { TraitFlag |= RValReferenceFlag; }

	constexpr void RemoveConstFlag() { TraitFlag &= ~ConstFlag; }
	constexpr void RemoveReferenceFlag() { TraitFlag &= ~ReferenceFlag; }
	constexpr void RemoveVolatileFlag() { TraitFlag &= ~VolatileFlag; }
	constexpr void RemoveRValReferenceFlag() { TraitFlag &= ~RValReferenceFlag; }

	constexpr void SetPointerAmount(uint16_t amount) { PointerAmount = amount; }
	constexpr uint32_t GetPointerAmount() const { return PointerAmount; }

	constexpr void SetArraySize(uint32_t size) { ArraySize = size; }
	constexpr uint32_t GetArraySize() const { return ArraySize; }

	constexpr bool IsConst() const { return TraitFlag & ConstFlag; }
	constexpr bool IsRef() const { return TraitFlag & ReferenceFlag; }
	constexpr bool IsVolatile() const { return TraitFlag & VolatileFlag; }
	constexpr bool IsRValReference() const { return TraitFlag & RValReferenceFlag; }
	constexpr bool IsPointer() const { return PointerAmount > 0; }
	constexpr bool IsArray() const { return ArraySize > 0; }
	constexpr bool IsRefOrPointer() const { return IsRef() || IsPointer() || IsRValReference(); }

	constexpr uint32_t GetSize() const { return IsRefOrPointer() ? sizeof(void*) : Type.GetTypeInfo().Size * GetArraySize(); }
	constexpr uint32_t GetAlignment() const { return IsRefOrPointer() ? sizeof(void*) : Type.GetTypeInfo().Alignment; }

	constexpr uint64_t GetHash() const { return Type.GetId() ^ ArraySize ^ (static_cast<uint64_t>(PointerAmount) << 32) ^ (static_cast<uint64_t>(TraitFlag) << 40); }

	friend bool operator==(VariableId const& lhs, VariableId const& rhs);

private:
	static constexpr uint32_t ConstFlag = 1 << 0;
	static constexpr uint32_t ReferenceFlag = 1 << 1;
	static constexpr uint32_t VolatileFlag = 1 << 2;
	static constexpr uint32_t RValReferenceFlag = 1 << 3;

	TypeId Type {};
	uint32_t ArraySize{};
	uint16_t PointerAmount{};
	uint8_t TraitFlag{};
};

template <typename t>
constexpr uint32_t CountPointers(uint32_t count = 0)
{
	if constexpr (std::is_pointer_v<t>)
	{
		return CountPointers<std::remove_pointer_t<t>>(++count);
	}
	else
	{
		return count;
	}
}

template <typename T>
static constexpr VariableId Create()
{
	using Type_RemovedExtents = std::remove_extent_t<T>;
	using Type_RemovedRef = std::remove_reference_t<Type_RemovedExtents>;
	using Type_RemovedPointers = remove_all_pointers_t<Type_RemovedRef>;

	using StrippedType = std::remove_cvref_t<Type_RemovedPointer>;
	RegisterType<StrippedType>();

	if (constexpr std::is_same_v<T, void>)
	{
		return VariableId(TypeId::Create<void>());
	}
	else
	{
		constexpr bool isRef = std::is_reference_v<T>;
		constexpr bool isRValRef = std::is_rvalue_reference_v<T>;
		constexpr bool isConst = std::is_const_v<Type_RemovedPointers>;
		constexpr bool isVolatile = std::is_volatile_v<Type_RemovedPointers>;

		const uint32_t pointerAmount = CountPointers<Type_RemovedPointers>();
		const uint32_t arraySize = sizeof(T) / sizeof(Type_RemovedExtents);

		auto variable = VariableId(TypeId::Create<StrippedType>());

		if (constexpr isRef) variable.SetReferenceFlag();
		if (constexpr isRValRef) variable.SetRValReferenceFlag();
		if (constexpr isConst) variable.SetConstFlag(); 
		if (constexpr isVolatile) variable.SetVolatileFlag();

		variable.SetPointerAmount(pointerAmount);
		variable.SetArraySize(arraySize);

		return variable;
	}
}

bool operator==(VariableId const& lhs, VariableId const& rhs)
{
	return lhs.Type == rhs.Type &&
		lhs.ArraySize == rhs.ArraySize &&
		lhs.PointerAmount == rhs.PointerAmount &&
		lhs.TraitFlag == rhs.TraitFlag;
}