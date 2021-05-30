#pragma once
#include "EnemyCharacterController.h"

class Coily;
class Pyramid;
class CoilyCharacterController final : public EnemyCharacterController
{
public:
	CoilyCharacterController();
	CoilyCharacterController* Clone() override { return new CoilyCharacterController{ *this }; }
	CoilyCharacterController(CoilyCharacterController&& other) = delete;
	CoilyCharacterController& operator=(CoilyCharacterController const& rhs) = delete;
	CoilyCharacterController& operator=(CoilyCharacterController&& rhs) = delete;
	virtual ~CoilyCharacterController() = default;
	
	void Initialize() override;
	void Update() override;

	void SetIdle(bool isIdle) { m_bIsIdle = isIdle; }
	void SetPyramid(Pyramid* pPyramid) { m_pPyramid = pPyramid; }
protected:
	ConnectionDirection ChooseDirection() override;

private:
	int static const MOVEMENT_QUEUE_SIZE{ 2 };
	ConnectionDirection m_MovementQueue[MOVEMENT_QUEUE_SIZE];
	int m_CurrentlyInQueue;
	
	Coily* m_pCoilyCharacter;
	Pyramid* m_pPyramid;
	
	bool m_bIsIdle;

	void FindQBert();
	void Move(ConnectionDirection dir);

	CoilyCharacterController(CoilyCharacterController const& other);
};