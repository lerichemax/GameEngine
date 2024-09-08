#pragma once
#include "AiControllerSystem.h"

enum class ConnectionDirection;
struct MovementComponent;
class AiControllerSystem : public System {

public:
	AiControllerSystem() = default;

	void SetSignature() override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Update() override;

	virtual void ChooseDirection(MovementComponent* const pMover) const;
};