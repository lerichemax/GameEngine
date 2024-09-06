#pragma once
#include "AiControllerSystem.h"

enum class ConnectionDirection;
struct MovementComponent;
class AiControllerSystem final : public System {

public:
	AiControllerSystem() = default;

	void SetSignature() override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Update() override;

private:
	void ChooseRandomDirection(MovementComponent* const pMover) const;
};