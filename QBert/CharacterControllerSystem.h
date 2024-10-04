#pragma once
#include "System.h"

class Registry;
class CharacterMovementSystem;
class CharacterControllerSystem : public System 
{
public:
	void SetSignature() const override;

protected:
	void Update() override;

	void Serialize(StreamWriter& writer) const override;
};
