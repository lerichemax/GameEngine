#pragma once
#include "Component.h"

#include "System.h"

struct TextRendererComponent;
struct FPSCounter : BehaviourComponent
{
	void Serialize(StreamWriter& writer) const override;

protected:
	void Update() override;

};