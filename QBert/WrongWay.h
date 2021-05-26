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
	void Update() override;
	void SetCurrentQube(Qube* pQube) { m_pCurrentQube = pQube; }
	bool IsEscheresqueRight() const { return m_bEscheresqueRight; }
	void MoveToCurrentQube() override;
	void JumpToQube(Qube* pNextQube) override;
	virtual void Die() override;
protected:
	ConnectionDirection ChooseDirection() override;

private:

	bool m_bEscheresqueRight;

};