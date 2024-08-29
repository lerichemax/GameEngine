#include "PCH.h"
#include "FPSCounter.h"

void FPSCounter::Serialize(StreamWriter& writer) const
{ 
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(FPSCounter)).hash_code()));
	
	Component::Serialize(writer);
};
