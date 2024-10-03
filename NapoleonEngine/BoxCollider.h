#pragma once
#include "ColliderComponent.h"

struct Rectangle;
class CircleCollider;
class BoxCollider final
{
public:
	explicit BoxCollider(unsigned int Width, unsigned int Height);

	BoxCollider(BoxCollider&& other) = delete;
	BoxCollider& operator=(BoxCollider const& rhs) = delete;
	BoxCollider& operator=(BoxCollider&& rhs) = delete;
	~BoxCollider();
		
	geo::Rectangle const& GetRectangle() const { return *m_pRectangle; }
	
protected:
	void Initialize();
	void Update();
		
	void CheckOverlap(ColliderComponent* pOther);

private:
	geo::Rectangle* m_pRectangle;

	BoxCollider(BoxCollider const& other);
		
	bool IsOverlapping(BoxCollider* pOther) const;
	bool IsOverlapping(CircleCollider* pOther) const;
};

