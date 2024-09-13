#pragma once
#include "Component.h"

enum class ConnectionDirection
{
	upRight,
	downRight,
	downLeft,
	upLeft,
	null
};

enum class Level;

struct QubeComponent : public Component
{
	QubeComponent();

	int static const MAX_NBR_CONNECTION{ 4 };
	static int const POINTS_FOR_FLIP{ 25 };

	Entity Connections[MAX_NBR_CONNECTION]; //0 :up-right, 1 : down-right, 2 : down -left, 3 : up- left
	//Qube* m_pEscheresqueRightConnections[MAX_NBR_CONNECTION];
	//Qube* m_pEscheresqueLeftConnections[MAX_NBR_CONNECTION];

	Entity ConnectionToDisk;
	std::unordered_set<Entity> Characters;

	Texture2D* const pDefaultText;
	Texture2D* const pIntermediateTexture;
	Texture2D* const pFlippedTexture;

	glm::vec2 CharacterPos{};
	//glm::vec2 m_EscheresqueLeftPos;
	//glm::vec2 m_EscheresqueRightPos;

	Level QubeLevel{ };

	bool bIsFlipped{};
	bool bIsLastRow{};
	bool bIsSideColumn{};

	unsigned int JumpCounter{};

	void Serialize(StreamWriter& writer) const override;

	bool HasConnection(ConnectionDirection direction) const;
	Entity GetConnection(ConnectionDirection direction) const;
};
