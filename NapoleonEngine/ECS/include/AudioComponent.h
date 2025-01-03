#pragma once
#include "Component.h"

struct AudioComponent final : public ecs::Component
{
	friend class AudioSystem;
public:
	AudioComponent();

	PROPERTY(float, m_Volume);
	PROPERTY(bool, m_Loop);

	void Play();

	void SetAudioClip(std::string const& clipLocation);
	
private:
	bool m_Play;
	int m_AudioId;

	PROPERTY(std::string, m_AudioClipLocation); //TODO : map clip location to id in the resource manager.

	void RetrieveAudioId();

};

SERIALIZE_CLASS(AudioComponent, ecs::Component)