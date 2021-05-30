#include "PCH.h"
#include  "PauseGameCommand.h"

#include "Timer.h"

PauseGameCommand::PauseGameCommand()
	:Command(),
	m_bIsPaused(false)
{
	
}

PauseGameCommand::PauseGameCommand(PauseGameCommand const& other)
	:m_bIsPaused(other.m_bIsPaused)
{
	
}
void PauseGameCommand::Execute()
{
	if (m_bIsPaused)
	{
		empire::Timer::GetInstance().SetTimeScale(1);
	}
	else
	{
		empire::Timer::GetInstance().SetTimeScale(0);
	}
	m_bIsPaused = !m_bIsPaused;
}
