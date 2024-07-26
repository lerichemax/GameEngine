#pragma once
#include "Command.h"
#include "Qube.h"

class CharacterMovement;

class MoveCommand final : public Command
{
public:
	MoveCommand() = default;
	MoveCommand(ConnectionDirection dir, std::shared_ptr<CharacterMovement> pChar);
	MoveCommand* Clone() { return new MoveCommand(*this); }
	MoveCommand(MoveCommand&& other) = delete;
	MoveCommand& operator=(MoveCommand const& rhs) = delete;
	MoveCommand& operator=(MoveCommand&& rhs) = delete;
	~MoveCommand() = default;

	void Execute() override;

	void Serialize(StreamWriter& writer) const override {};
	void Deserialize(JsonReader const* reader, SerializationMap& context) override {};

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {};

private:
	ConnectionDirection m_MoveDirection;
	std::shared_ptr<CharacterMovement> m_pMover;

	MoveCommand(MoveCommand const& other);
};
