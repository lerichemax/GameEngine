#pragma once
#include "Enemy.h"

class WrongWay final : public Enemy
{
public:
	WrongWay(bool escheresqueRight);

	WrongWay* Clone() override { return new WrongWay(*this); }
	~WrongWay() = default;

	void Initialize() override;
	void Move(ConnectionDirection direction) override;
	void MoveToCurrentQube() override{};
	void JumpToQube(Qube* pNextQube) override;
	virtual void Die() override;
	
	void SetCurrentQube(Qube* pQube) { m_pCurrentQube = pQube; }
	void SetEscheresqueRight(bool isRight) { m_bEscheresqueRight = isRight; }
	
	bool IsEscheresqueRight() const { return m_bEscheresqueRight; }


private:

	bool m_bEscheresqueRight;

};