#include "PCH.h"
#include "LoggingSoundSystem.h"

LoggingSoundSystem::~LoggingSoundSystem()
{
	delete m_pRealSoundSystem;
}

LoggingSoundSystem::LoggingSoundSystem(SoundInterface* pRealSS, bool isMuted)
	:m_pRealSoundSystem(pRealSS)
{
	m_bIsMuted = isMuted;
}

void LoggingSoundSystem::Play(unsigned int id, float volume)
{
	if (!m_bIsMuted)
	{
		m_pRealSoundSystem->Play(id, volume);
	}

	Debugger::GetInstance().Log("Playing " + std::to_string(id) + " at volume " + std::to_string(volume));
}

void LoggingSoundSystem::Update()
{
	m_pRealSoundSystem->Update();
}
