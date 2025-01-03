#include "PCH.h"
#include "SoundEffect.h"

#include <SDL_mixer.h>

ID SoundEffect::m_AutoIncrementId{};

SoundEffect::SoundEffect(Mix_Chunk* pMixChunk, std::string const& path) // gain ownership of pMixChunk
	:m_Id(),
	m_pMixChunk{ pMixChunk },
	m_Volume(),
	m_SavedVolume()
{
	m_AutoIncrementId++;
	m_Id = m_AutoIncrementId;
}

SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(m_pMixChunk);
	m_pMixChunk = nullptr;
	m_AutoIncrementId--;
}

bool SoundEffect::Play()
{
	if (m_pMixChunk != nullptr)
	{
		int channel{ Mix_PlayChannel(-1, m_pMixChunk, 0) };
		return channel == -1 ? false : true;
	}
	else
	{
		return false;
	}
}

bool SoundEffect::Play(float volume)
{
	SetVolume(volume);
	return Play();
}

void SoundEffect::SetVolume(float volume)
{
	if (m_pMixChunk != nullptr)
	{
		Mix_VolumeChunk(m_pMixChunk, (int)volume);
	}
}