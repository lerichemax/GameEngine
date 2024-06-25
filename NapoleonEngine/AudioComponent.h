#pragma once
#include "Component.h"

struct AudioComponent final : public ECS_Component
{
	friend class AudioSystem;
public:
	AudioComponent() = default;
	AudioComponent(unsigned int id, float volume);

	unsigned int m_AudioId;
	float m_Volume;
	bool m_Loop{ false };

	void Play();

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;
	
private:
	bool m_Play{ false };
};