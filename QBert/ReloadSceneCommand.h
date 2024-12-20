#pragma once
#include "Command.h"

class QBertScene;
class ReloadSceneCommand final : public Command
{
public:
	ReloadSceneCommand() = default;

	ReloadSceneCommand(ReloadSceneCommand const& other) = delete;
	ReloadSceneCommand(ReloadSceneCommand&& other) = delete;
	ReloadSceneCommand& operator=(ReloadSceneCommand const& rhs) = delete;
	ReloadSceneCommand& operator=(ReloadSceneCommand&& rhs) = delete;
	~ReloadSceneCommand() = default;

	void Execute(GameObject const& gObject) override;
};

SERIALIZE_CLASS(ReloadSceneCommand, IContextSerializable)