#pragma once
#include "Entity.h"
#include "Serializer.h"

#include <bitset>

class GameObject;

using ComponentType = uint8_t;

const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

struct Component : public IContextSerializable
{
	template<typename T> friend class ComponentArray;
	friend class Registry;
	friend class GameObject;

	Component() = default;
	Component(bool unique);

	virtual ~Component() = default;

	bool IsActive() const;
	bool IsUnique() const;

	void SetActive(bool isActive);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override;

	Entity GetEntity() const { return m_Entity; }

private:
	Entity m_Entity;

	bool m_IsActive{true};
	bool m_IsUnique{};
};