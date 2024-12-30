#include "PCH.h"
#include "AudioComponent.h"
#include "ResourceManager.h"

AudioComponent::AudioComponent()
	:m_AudioId{-1},
	m_Volume{50},
	m_Loop{false},
	m_Play{false}
{
}	

void AudioComponent::Play()
{
	if (m_AudioId == -1)
	{
		RetrieveAudioId();
	}
	m_Play = true;
}

void AudioComponent::SetAudioClip(std::string const& clipLocation)
{
	m_AudioClipLocation = clipLocation;
	RetrieveAudioId();
}

void AudioComponent::RetrieveAudioId()
{
	m_AudioId = ResourceManager::Get().GetEffect(m_AudioClipLocation);
}