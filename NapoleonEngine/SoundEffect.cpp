#include "PCH.h"
#include "SoundEffect.h"

using namespace empire;

ID SoundEffect::m_AutoIncrementId{};

SoundEffect::SoundEffect(std::string const& path)
	:m_pMixChunk{ Mix_LoadWAV(path.c_str()) }
{
	if (m_pMixChunk == nullptr)
	{
		std::string errorMsg = "SoundEffect: Failed to load " + path + ",\n SDL_mixer Error: " + Mix_GetError() + '\n';
		std::cerr << errorMsg;
	}
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

		int channel{ Mix_PlayChannel(-1, m_pMixChunk, 1) };
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