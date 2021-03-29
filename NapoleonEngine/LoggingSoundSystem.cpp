#include "PCH.h"
#include "LoggingSoundSystem.h"

using namespace empire;

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

	std::cout << "Playing " << id << " at volume " << volume << std::endl;
}

void LoggingSoundSystem::Update()
{
	m_pRealSoundSystem->Update();
}
