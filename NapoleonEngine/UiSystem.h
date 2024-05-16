#pragma once
#include "System.h"

class Coordinator;
class UiSystem : public System
{
public:
	UiSystem(Coordinator* const pRegistry);

	void Update(ComponentManager* const pComponentManager) override;

protected:
	std::shared_ptr<System> Clone() const override { return std::make_shared<UiSystem>(*this); }
};