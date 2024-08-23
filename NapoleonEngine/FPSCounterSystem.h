#pragma once
#include "System.h"

class FPSCounterSystem : public System
{
public:
	void SetSignature(Coordinator* const pRegistry) override;

protected:
	void Update(ComponentManager* const pComponentManager) override;

	void Serialize(StreamWriter& writer) const override;
};