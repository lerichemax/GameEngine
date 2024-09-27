#include "PCH.h"
#include "QubeComponent.h"
#include "AiControllerComponent.h"
#include "MovementComponent.h"

#include "ResourceManager.h"

#include "GameObject.h"

QubeComponent::QubeComponent()
	:Connections{ NULL_ENTITY, NULL_ENTITY, NULL_ENTITY, NULL_ENTITY },
	EscheresqueRightConnections{ NULL_ENTITY, NULL_ENTITY, NULL_ENTITY, NULL_ENTITY },
	EscheresqueLeftConnections{ NULL_ENTITY, NULL_ENTITY, NULL_ENTITY, NULL_ENTITY },
	pDefaultText(ResourceManager::Get().GetTexture("Textures/Qube/Qube.png")),
	pIntermediateTexture(ResourceManager::Get().GetTexture("Textures/Qube/Qube_Intermediate.png")),
	pFlippedTexture(ResourceManager::Get().GetTexture("Textures/Qube/Qube_Flipped.png")),
	ConnectionToDisk{ new DiskConnection{} }
{
}

QubeComponent::~QubeComponent()
{
	SAFE_DELETE(ConnectionToDisk);
}

void QubeComponent::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64_t>(std::type_index(typeid(QubeComponent)).hash_code()));
	
	Component::Serialize(writer);
}

bool QubeComponent::HasConnection(ConnectionDirection direction) const
{
	return Connections[static_cast<int>(direction)] != NULL_ENTITY;
}

bool QubeComponent::HasConnectionForMovementMode(MovementMode mode, ConnectionDirection direction) const
{
	switch (mode)
	{
	case MovementMode::Normal:
		return HasConnection(direction);
		break;
	case MovementMode::EscheresqueLeft:
		return EscheresqueLeftConnections[static_cast<int>(direction)] != NULL_ENTITY;
		break;
	case MovementMode::EscheresqueRight:
		return EscheresqueRightConnections[static_cast<int>(direction)] != NULL_ENTITY;
		break;
	}

	return false;
}

bool QubeComponent::HasConnectionToDisk() const
{
	return ConnectionToDisk->Disk != NULL_ENTITY && ConnectionToDisk->Direction != ConnectionDirection::null;
}

Entity QubeComponent::GetConnection(ConnectionDirection direction) const
{
	if (HasConnection(direction))
	{
		return Connections[static_cast<int>(direction)];
	}

	return NULL_ENTITY;
}

Entity QubeComponent::GetConnectionForMovementMode(MovementMode mode, ConnectionDirection direction) const
{
	if (HasConnectionForMovementMode(mode, direction))
	{
		switch (mode)
		{
		case MovementMode::Normal:
			return Connections[static_cast<int>(direction)];
			break;
		case MovementMode::EscheresqueRight:
			return EscheresqueRightConnections[static_cast<int>(direction)];
			break;
		case MovementMode::EscheresqueLeft:
			return EscheresqueLeftConnections[static_cast<int>(direction)];
			break;
		default:
			break;
		}
	}

	return NULL_ENTITY;
}

glm::vec2 const& QubeComponent::GetEnemyTopPosition(MovementMode mode) const
{
	switch (mode)
	{
	case MovementMode::Normal:
		return CharacterPos;
		break;
	case MovementMode::EscheresqueLeft:
		return EscheresqueLeftPos;
		break;
	case MovementMode::EscheresqueRight:
		return EscheresqueRightPos;
		break;
	}

	return CharacterPos;
}