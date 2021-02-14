#include "MiniginPCH.h"
#include "Component.h"

using namespace dae;

void Component::SetLocalTransform(const Transform& local)
{
	m_Local = local;
}
