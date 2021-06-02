#pragma once
#include "ColliderComponent.h"

namespace empire
{
	struct Rectangle;
	class CircleCollider;
	class BoxCollider final : public ColliderComponent
	{
	public:
		BoxCollider(unsigned int width, unsigned int height);
		BoxCollider* Clone() override { return new BoxCollider(*this); }

		BoxCollider(BoxCollider&& other) = delete;
		BoxCollider& operator=(BoxCollider const& rhs) = delete;
		BoxCollider& operator=(BoxCollider&& rhs) = delete;
		~BoxCollider();
		
		void Initialize() override;
		void Update() override;
		
		Rectangle const& GetRectangle() const { return *m_pRectangle; }
	
	protected:
		void CheckOverlap(ColliderComponent* pOther) override;

	private:
		Rectangle* m_pRectangle;

		BoxCollider(BoxCollider const& other);
		
		bool IsOverlapping(BoxCollider* pOther);
	};
}
