#pragma once
#include "Component.h"

class Command;
struct AiControllerComponent : public Component
{
	Command* pAiCommand;

	void Serialize(StreamWriter& writer) const override;
};