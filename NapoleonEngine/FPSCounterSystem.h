#pragma once
#include "System.h"

class FPSCounterSystem : public System
{
public:
	void SetSignature() override;

protected:
	void Update() override;

	void Serialize(StreamWriter& writer) const override;
};