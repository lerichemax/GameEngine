#pragma once
#include "Entity.h"
#include "Serializer.h"

#include <bitset>

class GameObject;

namespace ecs 
{
	using ComponentType = uint8_t;

	const ComponentType MAX_COMPONENTS = 32;

	using Signature = std::bitset<MAX_COMPONENTS>;

	struct Component : public ::IContextSerializable
	{
		template<typename T> friend class ComponentArray;
		friend class ComponentManager;
		friend class Registry;
		friend class ::GameObject;

		Component();
		Component(bool unique);

		virtual ~Component() = default;

		bool IsActive() const;
		bool IsUnique() const;

		void SetActive(bool isActive);

		Entity GetEntity() const { return m_Entity; }

	private:
		PROPERTY(Entity, m_Entity);

		PROPERTY(bool, m_IsActive);
		PROPERTY(bool, m_IsUnique);
	};

	SERIALIZE_CLASS(Component, ::IContextSerializable)
}