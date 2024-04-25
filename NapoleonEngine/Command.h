#pragma once

class GameObject;
class Command
{
public:
	Command() = default;

	virtual Command* Clone() = 0;
	Command(Command const& other) = delete;
	Command(Command&& other) = delete;
	Command& operator=(Command const& rhs) = delete;
	Command& operator=(Command&& rhs) = delete;
	virtual ~Command() = default;

	virtual void Execute() = 0;
};