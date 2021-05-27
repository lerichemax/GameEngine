#pragma once
#include "Enemy.h"

class Pyramid;
class Qube;
class Coily final : public Enemy
{
public:
	Coily();
	Coily* Clone() override { return new Coily(*this); }
	void Update() override;

	
	void Move(ConnectionDirection direction) override;
	void MeetCharacter(Character* pOther) override;
	void Die() override;
	
	void Transform();
	
	void SetIsIdle(bool isIdle);
	void SetQube(Qube* pQube) { m_pCurrentQube = pQube; }
	void SetPyramid(Pyramid* pPyramid) { m_pPyramid = pPyramid; }
	
	bool IsTransformed() const { return m_bIsTransformed; }

protected:
	ConnectionDirection ChooseDirection() override;

private:
	int static const MOVEMENT_QUEUE_SIZE{ 2 };
	bool m_bIsTransformed;
	bool m_bIsIdle;

	ConnectionDirection m_MovementQueue[MOVEMENT_QUEUE_SIZE];
	int m_CurrentlyInQueue;
	Pyramid* m_pPyramid;

	void InitMovementQueue();
	void SetDirectionTextures(ConnectionDirection dir);

	void Initialize() override;
	void FindQBert();

	Coily(Coily const& other);
};