#pragma once
#include "Enemy.h"

class Pyramid;
class Qube;
class Coily final : public Enemy
{
public:
	Coily(Pyramid* pPyramid);
	Coily* Clone() override { return new Coily(*this); }
	void Update() override;

	bool IsTransformed() const { return m_bIsTransformed; }
	
	void Transform();
	void Move(ConnectionDirection direction) override;
	void SetIsIdle(bool isIdle);
	void MeetCharacter(Character* pOther) override;
	void SetQube(Qube* pQube) { m_pCurrentQube = pQube; }
	void Die() override;
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
	void SetDirectionTextures(ConnectionDirection dir);

	void Initialize() override;
	void FindQBert();

	Coily(Coily const& other);
};