#pragma once
#include "Enemy.h"

class Pyramid;
class Coily final : public Enemy
{
public:
	Coily(Pyramid* pPyramid);

	void Update() override;
	void Transform();

	void Move(ConnectionDirection direction) override;
	void SetIsIdle(bool isIdle);

	//void Die() override;
protected:
	ConnectionDirection ChooseDirection() override;

private:
	int static const MOVEMENT_QUEUE_SIZE{ 2 };
	bool m_bIsTransformed{ false };
	bool m_bIsIdle{ false };

	ConnectionDirection m_MovementQueue[MOVEMENT_QUEUE_SIZE];
	int m_CurrentlyInQueue;
	Pyramid* m_pPyramid;

	void InitMovementQueue();
};