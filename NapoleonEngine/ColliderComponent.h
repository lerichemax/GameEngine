#pragma once
#include <functional>

#include "Component.h"

class GameObject;
class ColliderComponent : public Component
{
protected:
	enum class Type
	{
		Box,
		Circle
	};
	Type m_Type;
	
public:
	explicit ColliderComponent(Type type);
	virtual ~ColliderComponent();
		
	typedef std::function<void(GameObject* pObject, GameObject* pOtherObject)> CollisionFunction;

	void SetOnTriggerEnter(CollisionFunction function) { m_OnTriggerEnter = function; }
	void SetOnTriggerExit(CollisionFunction function) { m_OnTriggerExit = function; }
	void SetOnTriggerStay(CollisionFunction function) { m_OnTriggerStay = function; }
	void SetOnCollision(CollisionFunction function) { m_OnCollision = function; }

	bool IsTrigger() const { return m_bIsTrigger; }
	void SetIsTrigger(bool isTrigger) { m_bIsTrigger = isTrigger; }

	Type GetType() const { return m_Type; }
	
protected:
	friend class Scene;

	void Initialize() override;
	void Update() override {};
	ColliderComponent* Clone() const override = 0;
		
	virtual void CheckOverlap(ColliderComponent* pOther) = 0;
	void CallOverlapFunctions(bool isOverlapping, ColliderComponent* other);
		
	std::vector<ColliderComponent*> m_pOverlappingColliders;

	CollisionFunction m_OnTriggerEnter;
	CollisionFunction m_OnTriggerStay;
	CollisionFunction m_OnTriggerExit;
	CollisionFunction m_OnCollision;

	bool m_bIsTrigger;
};
