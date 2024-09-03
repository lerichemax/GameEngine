#include "PCH.h"
#include "QubeComponent.h"

#include "ResourceManager.h"

#include "GameObject.h"

QubeComponent::QubeComponent()
	:Connections{ EntityManager::NULL_ENTITY, EntityManager::NULL_ENTITY, EntityManager::NULL_ENTITY, EntityManager::NULL_ENTITY },
	pDefaultText(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube.png")),
	pIntermediateTexture(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Intermediate.png")),
	pFlippedTexture(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Flipped.png")),
	ConnectionToDisk{ EntityManager::NULL_ENTITY }

{

}
void QubeComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(QubeComponent)).hash_code()));
	
	Component::Serialize(writer);
}

bool QubeComponent::HasConnection(ConnectionDirection direction) const
{
	return Connections[static_cast<int>(direction)] != -1;
}

Entity QubeComponent::GetConnection(ConnectionDirection direction) const
{
	if (HasConnection(direction))
	{
		return Connections[static_cast<int>(direction)];
	}

	return -1;
}