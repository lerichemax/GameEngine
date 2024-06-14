#pragma once
#include "System.h"
#include "Coordinator.h"

class BehaviourSystem final : public System
{
public:
	void Initialize(ComponentManager* const pComponentManager);
	void Update(ComponentManager* const pComponentManager) override;

	void SetSignature(Coordinator* const pRegistry) override;
protected:
	std::shared_ptr<System> Clone() const override { return std::make_shared<BehaviourSystem>(*this); }
};
