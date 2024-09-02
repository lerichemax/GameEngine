#pragma once
#include "AiControllerSystem.h"

class AiControllerSystem final : public System {

public:
	AiControllerSystem() = default;

	void SetSignature() override;
	void Serialize(StreamWriter& writer) const override;

};