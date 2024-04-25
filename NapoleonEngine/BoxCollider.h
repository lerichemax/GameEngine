#pragma once
#include "ColliderComponent.h"

struct Rectangle;
class CircleCollider;
class BoxCollider final : public ColliderComponent
{
public:
	explicit BoxCollider(unsigned int width, unsigned int height);

	BoxCollider(BoxCollider&& other) = delete;
	BoxCollider& operator=(BoxCollider const& rhs) = delete;
	BoxCollider& operator=(BoxCollider&& rhs) = delete;
	~BoxCollider();
		
	geo::Rectangle const& GetRectangle() const { return *m_pRectangle; }
	
protected:
	void Initialize() override;
	void Update() override;
	BoxCollider* Clone() const override { return new BoxCollider(*this); }
		
	void CheckOverlap(ColliderComponent* pOther) override;

private:
	geo::Rectangle* m_pRectangle;

	BoxCollider(BoxCollider const& other);
		
	bool IsOverlapping(BoxCollider* pOther) const;
	bool IsOverlapping(CircleCollider* pOther) const;
};

