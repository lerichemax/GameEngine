#pragma once
#include "Character.h"
#include "EnemyObserver.h"

class Enemy : public Character
{
public:
	Enemy(int pointsForKill);
	virtual ~Enemy() override  = default;

	virtual Enemy* Clone() override { return new Enemy(*this); }
	
	virtual void Update() override;
	virtual void Move(ConnectionDirection direction) override;
	virtual void Die() override;

	int GetPointsForKill()const { return POINTS_FOR_KILL; }
	void SetQube(Qube* pQube) { m_pCurrentQube = pQube; }
protected:
	friend class EnemyManager;
	
	unsigned int const POINTS_FOR_KILL;
	
	virtual ConnectionDirection ChooseDirection();

	virtual void Initialize() override;
	virtual void MeetCharacter(Character* ) override {}
	void SetDirectionTextures(ConnectionDirection) override {}
	virtual void MoveToCurrentQube() override;
	virtual void JumpToQube(Qube* pTargetQube) override;
	
	Enemy(Enemy const& other)
		:Character(other),
		POINTS_FOR_KILL(other.POINTS_FOR_KILL),
		m_MoveTimer()
	{}

	float const MOVE_MAX_TIME{ 1.f };
	
	float m_MoveTimer;
};