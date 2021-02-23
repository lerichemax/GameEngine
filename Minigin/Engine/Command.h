#pragma once

enum class KeyActionState
{
	pressed,
	held,
	released
};

class Command
{
public:
	Command(KeyActionState state) : m_KeyAction{state}{}
	Command(Command const& other) = delete;
	Command(Command&& other) = delete;
	Command& operator=(Command const& rhs) = delete;
	Command& operator=(Command&& rhs) = delete;
	virtual ~Command() = default;
	
	virtual bool Execute() = 0;
	KeyActionState GetKeyActionState() const { return m_KeyAction; }
protected:
	KeyActionState m_KeyAction;
};

class FireCommand final : public Command
{
public:
	FireCommand(KeyActionState state = KeyActionState::pressed) : Command(state){}
	FireCommand(FireCommand const& other) = delete;
	FireCommand(FireCommand&& other) = delete;
	FireCommand& operator=(FireCommand const& rhs) = delete;
	FireCommand& operator=(FireCommand&& rhs) = delete;
	~FireCommand() override = default;

	bool Execute() override;
};

class DuckCommand final : public Command
{
public:
	DuckCommand(KeyActionState state = KeyActionState::pressed) : Command(state) {}
	DuckCommand(DuckCommand const& other) = delete;
	DuckCommand(DuckCommand&& other) = delete;
	DuckCommand& operator=(DuckCommand const& rhs) = delete;
	DuckCommand& operator=(DuckCommand&& rhs) = delete;
	~DuckCommand() override = default;

	bool Execute() override;
};

class JumpCommand final : public Command
{
public:
	JumpCommand(KeyActionState state = KeyActionState::pressed) : Command(state) {}
	JumpCommand(JumpCommand const& other) = delete;
	JumpCommand(JumpCommand&& other) = delete;
	JumpCommand& operator=(JumpCommand const& rhs) = delete;
	JumpCommand& operator=(JumpCommand&& rhs) = delete;
	~JumpCommand() override = default;

	bool Execute() override;
};

class FartCommand final : public Command
{
public:
	FartCommand(KeyActionState state = KeyActionState::pressed) : Command(state) {}
	FartCommand(FartCommand const& other) = delete;
	FartCommand(FartCommand&& other) = delete;
	FartCommand& operator=(FartCommand const& rhs) = delete;
	FartCommand& operator=(FartCommand&& rhs) = delete;
	~FartCommand() override = default;

	bool Execute() override;
};

class QuitCommand final : public Command
{
public:
	QuitCommand(KeyActionState state = KeyActionState::pressed) : Command(state) {}
	QuitCommand(QuitCommand const& other) = delete;
	QuitCommand(QuitCommand&& other) = delete;
	QuitCommand& operator=(QuitCommand const& rhs) = delete;
	QuitCommand& operator=(QuitCommand&& rhs) = delete;
	~QuitCommand() override = default;

	bool Execute() override;
};
