#include "PCH.h"
#include "QbertComponent.h"
#include "AudioComponent.h"

QbertComponent::QbertComponent()
	:Disk{NULL_ENTITY}
{
}

void QbertComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(QbertComponent)).hash_code()));
	writer.WriteInt("jumpId", pJumpSound->GetId());
	writer.WriteInt("fallId", pFallSound->GetId());
	writer.WriteInt("swearId", pSwearSound->GetId());
	writer.WriteInt("playerNbr", PlayerNumber);

	Component::Serialize(writer);
}

void QbertComponent::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	Component::RestoreContext(reader, context);

	int id = -1;
	reader->ReadInt("jumpId", id);
	pJumpSound = context.GetRef<AudioComponent>(id);
	reader->ReadInt("fallId", id);
	pFallSound = context.GetRef<AudioComponent>(id);
	reader->ReadInt("swearId", id);
	pSwearSound = context.GetRef<AudioComponent>(id);

	reader->ReadInt("playerNbr", PlayerNumber);
}