#pragma once
#include "Component.h"

enum class ConnectionDirection;
class Enemy;
class EnemyCharacterController : public Component
{
public:
	EnemyCharacterController();
	EnemyCharacterController(EnemyCharacterController&& other) = delete;
	EnemyCharacterController& operator=(EnemyCharacterController const& rhs) = delete;
	EnemyCharacterController& operator=(EnemyCharacterController&& rhs) = delete;
	virtual ~EnemyCharacterController() = default;

	void Initialize();


protected:
	friend class EnemyOnQubeState;
	
	float const MOVE_MAX_TIME{ 0.5f };
	float m_MoveTimer;

	Enemy * m_pEnemyCharacter;
	
	virtual ConnectionDirection ChooseDirection();

	EnemyCharacterController(EnemyCharacterController const& other);
};
