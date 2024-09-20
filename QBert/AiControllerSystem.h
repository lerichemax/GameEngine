#pragma once
#include "AiControllerSystem.h"

enum class ConnectionDirection;
struct AiControllerComponent;
struct MovementComponent;
class AiControllerSystem : public System {

public:
	AiControllerSystem() = default;

	void SetSignature() const override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Start() override;
	void Update() override;

	void HandleAi(MovementComponent* const pMovement, AiControllerComponent* const pController);

	virtual void ChooseDirection(MovementComponent* const pMover) const;

private:
	bool m_bIsPaused;
};