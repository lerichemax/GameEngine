#pragma once
#include "ColliderComponent.h"

namespace empire
{
	struct Circle;
	class CircleCollider final : public ColliderComponent
	{
	public:
		CircleCollider();
	
	protected:
		void CheckOverlap(ColliderComponent* pOther) override;

	private:
		Circle* m_pCircle;

		Circle const& GetCircle() const { return *m_pCircle; }
	};
}