#pragma once
#include "EnemyCharacterController.h"

class Coily;
class PyramidSystem;
class CoilyCharacterController final : public EnemyCharacterController
{
public:
	CoilyCharacterController();
	CoilyCharacterController(CoilyCharacterController&& other) = delete;
	CoilyCharacterController& operator=(CoilyCharacterController const& rhs) = delete;
	CoilyCharacterController& operator=(CoilyCharacterController&& rhs) = delete;
	virtual ~CoilyCharacterController() = default;
	
	void Initialize();

	void SetIdle(bool isIdle);
	void SetPyramid(PyramidSystem* pPyramid) { m_pPyramid = pPyramid; }
	void FindQBert();
protected:
	ConnectionDirection ChooseDirection() override;

private:
	friend class CoilyOnQubeState;
	unsigned int static const MOVEMENT_QUEUE_SIZE{ 3 };
	
	ConnectionDirection MovementQueue[MOVEMENT_QUEUE_SIZE];
	
	int CurrentlyInQueue;
	
	Coily* m_pCoilyCharacter;
	PyramidSystem* m_pPyramid;
	
	bool m_bIsIdle;

	void Move(ConnectionDirection dir);

	CoilyCharacterController(CoilyCharacterController const& other);
};