#pragma once
#include "Command.h"

class QuitGameCommand final : public Command
{
public:
	QuitGameCommand() = default;
	QuitGameCommand* Clone() override { return new QuitGameCommand(*this); }
	QuitGameCommand(QuitGameCommand&& other) = delete;
	QuitGameCommand& operator=(QuitGameCommand const& rhs) = delete;
	QuitGameCommand& operator=(QuitGameCommand&& rhs) = delete;
	void Execute() override;

	void Serialize(StreamWriter& writer) const override {};
	void Deserialize(JsonReader const* reader, SerializationMap& context) override {};

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {};



private:

	QuitGameCommand(QuitGameCommand const& ){}
	
};