#pragma once
#include "Entity.h"
#include "Serializer.h"

#include <set>

class ecs::Registry;

namespace ecs
{
	class System
	{
		friend class Registry;
		friend class Scene;

	public:
		virtual ~System() = default;

	protected:
		virtual void Initialize() {};
		virtual void Start() {};
		virtual void Update() {};

		System() = default;

		Registry* m_pRegistry;
	};
}
