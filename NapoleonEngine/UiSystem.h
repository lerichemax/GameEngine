#pragma once
#include "System.h"

class Coordinator;
class UiSystem : public System
{
public:
	UiSystem(Coordinator* const pRegistry);

	void Update(ComponentManager* const pComponentManager) override;
};