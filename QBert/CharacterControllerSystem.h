#pragma once
#include "System.h"

class Coordinator;
class CharacterMovementSystem;
class CharacterControllerSystem : public System 
{
public:
	void SetSignature() override;

protected:
	void Update() override;

	void Serialize(StreamWriter& writer) const override;
};
