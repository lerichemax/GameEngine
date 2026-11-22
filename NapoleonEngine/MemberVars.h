#pragma once

#include "VariableId.h"

#include <set>

class MemberId final 
{
public:
	struct MemberInfo
	{
		std::string Name{};
		VariableId VariableId{};
		uint32_t Offset{};
		uint32_t Size{};
		uint32_t Alignment{};

		constexpr bool operator<(MemberInfo const& other) const
		{
			return Offset < other.Offset;
		}
	};

	constexpr MemberId() = default;
	constexpr MemberId(TypeId id, uint32_t Offset) 
		: Id(id)
		, Offset(Offset)
	{
	}

	constexpr TypeId GetTypeId() const { return Id; }
	constexpr uint32_t GetOffset() const { return Offset; }

	MemberInfo const& GetMemberInfo() const;
	std::string const& GetName() const { return GetMemberInfo().Name; }
	VariableId GetVariableId() const { return GetMemberInfo().VariableId; }
	uint32_t GetSize() const { return GetMemberInfo().Size; }
	uint32_t GetAlignment() const { return GetMemberInfo().Alignment; }

	template <typename Class, typename Field>
	static MemberId RegisterField(std::string const& fieldName, uint32_t offset);

	template <typename Field>
	static MemberId RegisterField(TypeId classId, std::string const& fieldName, uint32_t offset);

	static MemberId RegisterField(TypeId classId, VariableId memberId, std::string const& fieldName, uint32_t offset, uint32_t size, uint32_t alignment);

	static std::set<MemberInfo> const& GetMemberInfos(TypeId id) { return GetStatics().MemberInfos[id.GetId()]; }

	static const auto& GetAllMemberInfos() { return GetStatics().MemberInfos; }

private:
	struct StaticData
	{
		std::unordered_map<uint64_t, std::set<MemberInfo>> MemberInfos{};
	};

	static StaticData& GetStatics()
	{
		static StaticData data{};
		return data;
	}

	TypeId Id{};
	uint32_t Offset{};
};

struct RegisterMember final
{
	RegisterMember(TypeId classId, VariableId memberId, std::string const& fieldName, uint32_t offset, uint32_t size, uint32_t alignment)
	{
		MemberId::RegisterField(classId, memberId, fieldName, offset, size, alignment);
	}
};

#define MEMBER(TYPE, FIELD) inline static RegisterMember TYPE##FIELD(TypeId::Create<TYPE>(), VariableId::Create<decltype(TYPE::FIELD)>(), #FIELD, offsetof(TYPE, FIELD), sizeof(decltype(TYPE::FIELD)), alignof(decltype(TYPE::FIELD)));

inline MemberId::MemberInfo const& MemberId::GetMemberInfo() const
{
	auto& memberInfos = GetMemberInfos(Id);

	auto it = memberInfos.find(MemberInfo{ "", {}, Offset, 0, 0 });

	assert(it != memberInfos.end());

	return *it;
}

inline MemberId MemberId::RegisterField(TypeId classId, VariableId memberId, std::string const& fieldName, uint32_t offset, uint32_t size, uint32_t alignment)
{
	MemberInfo memberInfo{ fieldName, memberId, offset, size, alignment };

	GetStatics().MemberInfos[classId.GetId()].emplace(memberInfo);

	return MemberId(classId, offset);
}

template <typename Class, typename Field>
static MemberId RegisterField(std::string const& fieldName, uint32_t offset)
{
	auto registeredClass = RegisterType<Class>();
	auto registeredField = RegisterType<Field>();

	return MemberId::RegisterField(TypeId::Create<Class>(), VariableId::Create<Field>(), fieldName, offset, sizeof(Field), alignof(Field));
}

template <typename Field>
static MemberId RegisterField(TypeId classId, std::string const& fieldName, uint32_t offset)
{
	auto registeredField = RegisterType<Field>();

	return MemberId::RegisterField(classId, VariableId::Create<Field>(), fieldName, offset, sizeof(Field), alignof(Field));
}