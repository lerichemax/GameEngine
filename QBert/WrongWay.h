#pragma once
#include "Enemy.h"

class WrongWay final : public Enemy
{
public:
	WrongWay(bool escheresqueRight);

	WrongWay* Clone() override { return new WrongWay(*this); }
	~WrongWay() = default;

	void Move(ConnectionDirection direction) override;
private:

	bool m_bEscheresqueRight;
};