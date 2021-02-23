#include "MiniginPCH.h"
#include "Command.h"
#include <iostream>

bool FireCommand::Execute()
{
	std::cout << "Fire\n";
	return true;
}

bool JumpCommand::Execute()
{
	std::cout << "Jump\n";
	return true;
}

bool DuckCommand::Execute()
{
	std::cout << "Duck\n";
	return true;
}

bool FartCommand::Execute()
{
	std::cout << "Fart\n";
	return true;
}

bool QuitCommand::Execute()
{
	return false;
}