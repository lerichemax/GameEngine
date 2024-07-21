#include "PCH.h"
#include "AudioComponent.h"

AudioComponent::AudioComponent(unsigned int id, float volume)
	:m_AudioId{id},
	m_Volume{volume}
{
}	

void AudioComponent::Play()
{
	m_Play = true;
}

void AudioComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(AudioComponent).name());
	writer.WriteInt("id", m_AudioId);
	writer.WriteDouble("volume", m_Volume);
	writer.WriteBool("loop", m_Loop);
	writer.WriteBool("play", m_Play);
}

void AudioComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	int id;
	reader->ReadInt("id", id);
	m_AudioId = id;

	reader->ReadDouble("volume", m_Volume);
	reader->ReadBool("loop", m_Loop);
	reader->ReadBool("play", m_Play);
}