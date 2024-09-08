#pragma once
#include "AiControllerSystem.h"

class PyramidSystem;
class CoilySystem final : public AiControllerSystem {

public:
	CoilySystem() = default;

	void SetSignature() override;
	void Serialize(StreamWriter& writer) const override;

protected:
	void Start() override;
	void ChooseDirection(MovementComponent* const pMover) const override;

private:
	PyramidSystem* m_pPyramid;

	Entity m_Qbert;

	void HandleCoilyTransform(Entity entity);
};