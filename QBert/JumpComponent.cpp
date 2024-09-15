#include "PCH.h"
#include "JumpComponent.h"

void JumpComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(JumpComponent)).hash_code()));

	Component::Serialize(writer);
}

void JumpComponent::Reset()
{
	bJumpDown = false;
	bIsJumping = false;
	bIsFalling = false;
}
