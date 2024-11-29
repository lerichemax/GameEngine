#pragma once
#include "Component.h"

struct ScriptComponent : public Component
{
	PROPERTY(std::string, ScriptFile);

};

SERIALIZE_CLASS(ScriptComponent, ecs::Component)