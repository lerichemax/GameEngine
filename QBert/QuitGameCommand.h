#pragma once
#include "Command.h"

class QuitGameCommand final : public Command
{
public:
	QuitGameCommand() = default;

	void Execute(GameObject const& gObject) override;

};

SERIALIZE_CLASS(QuitGameCommand, IContextSerializable)