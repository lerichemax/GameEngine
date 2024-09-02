#include "PCH.h"
#include "{{CLASS_NAME}}.h"

void {{CLASS_NAME}}::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid({{CLASS_NAME}})).hash_code()));
}

void {{CLASS_NAME}}::SetSignature()
{
	Signature signature;

	m_pRegistry->SetSystemSignature<{{CLASS_NAME}}>(signature);
}