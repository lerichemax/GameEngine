#pragma once
#include "AiControllerSystem.h"

#include "Event.h"

class PyramidSystem;
class CoilySystem final : public AiControllerSystem {

public:
	CoilySystem() = default;

	EventHandler<CoilySystem, Entity> OnCoilyTransformed;

	void SetSignature() const override;
	void Serialize(StreamWriter& writer) const override;

	void ResetCoily(Entity entity);

protected:
	void Start() override;
	void Update() override;
	void ChooseDirection(MovementComponent* const pMover) const override;

private:
	PyramidSystem* m_pPyramid{};

	Entity m_Qbert{};

	void HandleJumpToDeath(Entity coilyEntity);
	void HandleCoilyTransform(Entity entity);
	void SearchForQbert(Entity entity);
};