#pragma once
#include "System.h"

class Coordinator;
class UiSystem : public System
{
public:
	void Update(ComponentManager* const pComponentManager) override;

	void SetSignature(Coordinator* const pRegistry) override;

protected:
	std::shared_ptr<System> Clone() const override { return std::make_shared<UiSystem>(*this); }
};