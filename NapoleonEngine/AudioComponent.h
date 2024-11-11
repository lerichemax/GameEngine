#pragma once
#include "Component.h"

struct AudioComponent final : public ecs::Component
{
	friend class AudioSystem;
public:
	AudioComponent();

	float m_Volume = 50;
	bool m_Loop{ false };

	void Play();

	void SetAudioClip(std::string const& clipLocation);

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
	
private:
	bool m_Play{ false };
	int m_AudioId;
};