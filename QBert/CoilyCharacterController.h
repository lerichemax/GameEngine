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

	void SetIdle(bool isIdle);
	void SetPyramid(Pyramid* pPyramid) { m_pPyramid = pPyramid; }
	void FindQBert();
protected:
	ConnectionDirection ChooseDirection() override;

private:
	int static const MOVEMENT_QUEUE_SIZE{ 3 };
	
	ConnectionDirection m_MovementQueue[MOVEMENT_QUEUE_SIZE];
	
	int m_CurrentlyInQueue;
	
	Coily* m_pCoilyCharacter;
	Pyramid* m_pPyramid;
	
	bool m_bIsIdle;

	void Move(ConnectionDirection dir);

	CoilyCharacterController(CoilyCharacterController const& other);
};