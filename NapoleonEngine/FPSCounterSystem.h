#pragma once
#include "System.h"

class FPSCounterSystem : public System
{
public:
	void SetSignature(Coordinator* const pRegistry) override;

protected:
	void Update() override;

	void Serialize(StreamWriter& writer) const override;
};