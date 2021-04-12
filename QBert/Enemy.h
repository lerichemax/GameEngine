#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
	Enemy();

	virtual void Update() override;
	virtual void Move(ConnectionDirection direction) override;
	virtual void Die() override;

protected:
	virtual ConnectionDirection ChooseDirection() const;

private:
	float const MOVE_MAX_TIME{ 1.f };

	float m_MoveTimer;

};