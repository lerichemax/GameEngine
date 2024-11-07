#include "PCH.h"
#include "FPSCounter.h"

void FPSCounter::Serialize(StreamWriter& writer) const
{ 
	writer.WriteString(std::string{ "type" }, typeid(FPSCounter).name());
	
	Component::Serialize(writer);
};
