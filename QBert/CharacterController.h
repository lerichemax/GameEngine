#pragma once
#include "Component.h"

class CharacterController final : public empire::Component
{
public:
	CharacterController() = default;
	CharacterController* Clone() override { return new CharacterController(*this); }

	CharacterController(CharacterController&& other) = delete;
	CharacterController& operator=(CharacterController const& rhs) = delete;
	CharacterController& operator=(CharacterController&& rhs) = delete;
	~CharacterController() = default;

	void Initialize() override;
	void Update() override;

private:
	CharacterController(CharacterController const& ){}
};