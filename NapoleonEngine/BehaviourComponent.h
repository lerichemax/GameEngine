#pragma once
#include "Component.h"

class BehaviourComponent : public Component
{
	friend class BehaviourSystem;

protected:
	virtual void Initialize() {};
	virtual void Start() {};
	virtual void Update() {};
};
