#pragma once
#include "ColliderComponent.h"

namespace empire
{
	struct Rectangle;
	class CircleCollider;
	class BoxCollider final : public ColliderComponent
	{
	public:
		BoxCollider(float width, float height);

		void Initialize() override;
	protected:
		void CheckOverlap(ColliderComponent* pOther) override;

	private:
		Rectangle* m_pRectangle;

		Rectangle const& GetRectangle() const { return *m_pRectangle; }
		
		bool IsOverlapping(BoxCollider* pOther);
		//void IsOverlapping(CircleCollider* pOther);
	};
}
