#include "PCH.h"
#include "AiControllerSystem.h"

void AiControllerSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(AiControllerSystem)).hash_code()));
}

void AiControllerSystem::SetSignature()
{
	Signature signature;

	m_pRegistry->SetSystemSignature<AiControllerSystem>(signature);
}