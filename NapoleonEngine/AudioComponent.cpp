#include "PCH.h"
#include "AudioComponent.h"
#include "ResourceManager.h"

AudioComponent::AudioComponent()
	:m_AudioId{-1},
	m_Volume{50}
{
}	

void AudioComponent::Play()
{
	m_Play = true;
}

void AudioComponent::SetAudioClip(std::string const& clipLocation)
{
	m_AudioId = ResourceManager::Get().GetEffect(clipLocation);
}

void AudioComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt("id", m_AudioId);
	writer.WriteDouble("volume", m_Volume);
	writer.WriteBool("loop", m_Loop);
	writer.WriteBool("play", m_Play);

	Component::Serialize(writer);
}

void AudioComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{
	int id;
	reader->ReadInt("id", id);
	m_AudioId = id;

	reader->ReadDouble("volume", m_Volume);
	reader->ReadBool("loop", m_Loop);
	reader->ReadBool("play", m_Play);

	Component::Deserialize(reader, context);
}