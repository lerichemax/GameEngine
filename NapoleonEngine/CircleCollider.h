#pragma once
#include "ColliderComponent.h"

struct Circle;
class BoxCollider;
class CircleCollider final
{
public:
	CircleCollider(glm::vec2 const& center, unsigned int radius);

	CircleCollider(CircleCollider&& other) = delete;
	CircleCollider& operator=(CircleCollider const& rhs) = delete;
	CircleCollider& operator=(CircleCollider&& rhs) = delete;
	~CircleCollider();
		
	geo::Circle const& GetCircle() const { return *m_pCircle; }
	
protected:
	void CheckOverlap(ColliderComponent* pOther);
		

private:
	geo::Circle* m_pCircle;

	bool IsOverlapping(BoxCollider* pOther) const;
	bool IsOverlapping(CircleCollider* pOther) const;
		
	CircleCollider(CircleCollider const& other);
};
