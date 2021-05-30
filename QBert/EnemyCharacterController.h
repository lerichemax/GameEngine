#pragma once
#include "Component.h"

enum class ConnectionDirection;
class Enemy;
class EnemyCharacterController : public empire::Component
{
public:
	EnemyCharacterController();
	EnemyCharacterController* Clone() override { return new EnemyCharacterController{ *this }; }
	EnemyCharacterController(EnemyCharacterController&& other) = delete;
	EnemyCharacterController& operator=(EnemyCharacterController const& rhs) = delete;
	EnemyCharacterController& operator=(EnemyCharacterController&& rhs) = delete;
	virtual ~EnemyCharacterController() = default;

	void Initialize() override;
	void Update() override;

protected:
	float const MOVE_MAX_TIME{ 0.5f };
	float m_MoveTimer;

	Enemy * m_pEnemyCharacter;
	
	virtual ConnectionDirection ChooseDirection();

	EnemyCharacterController(EnemyCharacterController const& other);
};
