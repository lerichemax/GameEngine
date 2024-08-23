#pragma once
#include "System.h"

class Coordinator;
class CharacterMovementSystem;
class CharacterControllerSystem : public System 
{
public:
	void SetSignature(Coordinator* const pRegistry) override;

protected:
	void Start();
	void Update(ComponentManager* const pComponentManager) override;

	void Serialize(StreamWriter& writer) const override;

//private:
//	CharacterMovementSystem* m_pCharacterMover;
};
