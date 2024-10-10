#pragma once
#include "Command.h"

class QuitGameCommand final : public Command
{
public:
	QuitGameCommand() = default;

	QuitGameCommand(QuitGameCommand const&) = delete;
	QuitGameCommand(QuitGameCommand&& other) = delete;
	QuitGameCommand& operator=(QuitGameCommand const& rhs) = delete;
	QuitGameCommand& operator=(QuitGameCommand&& rhs) = delete;

	void Execute(GameObject const& gObject) override;

	void Serialize(StreamWriter& writer) const override;
};