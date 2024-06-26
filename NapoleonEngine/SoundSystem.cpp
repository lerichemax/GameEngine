#include "PCH.h"
#include "SoundSystem.h"
#include "SoundEffect.h"

#include <algorithm>
#include <thread>

SoundSystem::SoundSystem(bool isMuted)
	:m_Pending{},
	m_NbrPending(),
	m_pSounds()
{
	m_bIsMuted = isMuted;
}

SoundSystem::~SoundSystem()
{
	for (auto& pair : m_pSounds)
	{
		delete pair.second;
	}
	m_pSounds.clear();
}

void SoundSystem::Update()
{
	if (m_NbrPending == 0)
	{
		return;
	}
	
	std::thread updateThread([this]()
		{
			std::lock_guard<std::mutex> guard(m_Mutex);
			for (unsigned int i{}; i < m_NbrPending; i++)
			{
				m_pSounds[m_Pending[i].id]->Play(m_Pending[i].volume);
			}

			m_NbrPending = 0;
		});
	updateThread.join();
}

void SoundSystem::Play(unsigned int id, float volume)
{
	if (!m_bIsMuted && m_NbrPending < MAX_PENDING - 1)
	{
		m_Pending[m_NbrPending] = ToPlay{ id,volume };
		m_NbrPending++;
	}
}

ID SoundSystem::AddEffect(SoundEffect* pEffect)
{
	m_pSounds.insert(std::make_pair(pEffect->m_Id, pEffect));
	return pEffect->m_Id;
}

ID SoundSystem::AddEffect(std::string const& path)
{
	auto* pEffect = new SoundEffect{ path };
	m_pSounds.insert(std::make_pair(pEffect->m_Id, pEffect));
	return pEffect->m_Id;
}