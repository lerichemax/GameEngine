#pragma once
#include "Component.h"

enum class MovementMode;
enum class ConnectionDirection
{
	upRight,
	downRight,
	downLeft,
	upLeft,
	null
};

struct DiskConnection 
{
	Entity Disk{NULL_ENTITY};
	ConnectionDirection Direction{ ConnectionDirection::null };
};

enum class Level;
enum class EnemyType;

struct QubeComponent : public Component
{
	QubeComponent();
	~QubeComponent();

	int static const MAX_NBR_CONNECTION{ 4 };
	static int const POINTS_FOR_FLIP{ 25 };

	Entity Connections[MAX_NBR_CONNECTION]; //0 :up-right, 1 : down-right, 2 : down -left, 3 : up- left
	Entity EscheresqueRightConnections[MAX_NBR_CONNECTION];
	Entity EscheresqueLeftConnections[MAX_NBR_CONNECTION];

	DiskConnection* ConnectionToDisk;

	std::unordered_set<Entity> Characters;

	Texture2D* const pDefaultText;
	Texture2D* const pIntermediateTexture;
	Texture2D* const pFlippedTexture;

	glm::vec2 CharacterPos{};
	glm::vec2 EscheresqueLeftPos{};
	glm::vec2 EscheresqueRightPos{};

	Level QubeLevel{ };

	bool bIsFlipped{};
	bool bIsLastRow{};
	bool bIsSideColumn{};

	unsigned int JumpCounter{};

	void Serialize(StreamWriter& writer) const override;

	bool HasConnection(ConnectionDirection direction) const;
	bool HasConnectionForMovementMode(MovementMode mode, ConnectionDirection direction) const;
	bool HasConnectionToDisk() const;
	Entity GetConnection(ConnectionDirection direction) const;
	Entity GetConnectionForMovementMode(MovementMode mode, ConnectionDirection direction) const;
	glm::vec2 const& GetEnemyTopPosition(MovementMode mode) const;
};
