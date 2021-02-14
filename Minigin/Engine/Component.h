#pragma once
#include "Transform.h"

namespace dae
{
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;
		void SetLocalTransform(const Transform& local);
	private:
		Transform m_Local;
		virtual void MakeItPolymorphic() const = 0; //TEMP
	};

}