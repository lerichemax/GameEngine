#include "PCH.h"
#include "BehaviourComponent.h"

void BehaviourComponent::Serialize(StreamWriter& writer) const
{ 
	writer.WriteBool("behaviour", true);
	ECS_Component::Serialize(writer); 
};
void BehaviourComponent::Deserialize(JsonReader const* reader, SerializationMap& context)
{ 
	ECS_Component::Deserialize(reader, context); 
};

void BehaviourComponent::RestoreContext(JsonReader const* reader, SerializationMap const& context)
{
	ECS_Component::RestoreContext(reader, context); 
};