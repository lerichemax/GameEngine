#include "PCH.h"
#include "System.h"

#pragma warning(push)
#pragma warning(disable : 4100)


System::System(System const& other)
	:m_Entities{}
{
}

void System::AddEntity(Entity entity)
{
	m_Entities.insert(entity);
}

#pragma warning(pop)