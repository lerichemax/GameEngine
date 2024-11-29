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
	PROPERTY(bool, m_Play);
	PROPERTY(int, m_AudioId);
};

SERIALIZE_CLASS(AudioComponent, ecs::Component)