#pragma once
#include "Command.h"

class QBertScene;
class ReloadSceneCommand final : public Command
{
public:
	ReloadSceneCommand() = default;
	~ReloadSceneCommand() = default;

	void Execute(GameObject const& gObject) override;
};

SERIALIZE_CLASS(ReloadSceneCommand, IContextSerializable)