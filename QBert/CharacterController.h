#pragma once
#include "BehaviourComponent.h"

class CharacterMovement;
class CharacterController : public BehaviourComponent
{
public:
	void Serialize(StreamWriter& writer) const override;

protected:
	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	std::shared_ptr<CharacterMovement> m_pMover;
};
