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
	void Update() override;
	void ChooseDirection(MovementComponent* const pMover) const override;


private:
	PyramidSystem* m_pPyramid{};

	Entity m_Qbert{};

	void HandleJumpToDeath(Entity coilyEntity);
	void HandleCoilyTransform(Entity entity);
	void CheckForReset(Entity entity);
	void ResetCoily(Entity entity);
	void SearchForQbert(Entity entity);
};