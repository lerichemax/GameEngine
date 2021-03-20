#include "PCH.h"
#include "SoundSystem.h"
#include "SoundEffect.h"

#include <iostream>
#include <algorithm>

using namespace empire;

SoundSystem::SoundSystem(bool isMuted)
	:m_pSounds(),
	m_bIsMuted(isMuted)
{
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
	
	for (unsigned int i{}; i < m_NbrPending; i++)
	{
		m_pSounds[m_Pending[i].id]->Play(m_Pending[i].volume);
	}

	m_NbrPending = 0;
}

void SoundSystem::Play(unsigned int id, float volume)
{
	if (!m_bIsMuted && m_NbrPending < MAX_PENDING-1)
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
	auto pEffect = new SoundEffect{ path };
	m_pSounds.insert(std::make_pair(pEffect->m_Id, pEffect));
	return pEffect->m_Id;
}

void SoundSystem::Mute()
{
	m_bIsMuted = true;
	std::for_each(m_pSounds.begin(), m_pSounds.end(), [](auto& pair)
		{
			pair.second->Mute();
		});
}

void SoundSystem::UnMute()
{
	m_bIsMuted = false;
	std::for_each(m_pSounds.begin(), m_pSounds.end(), [](auto& pair)
		{
			pair.second->UnMute();
		});
}