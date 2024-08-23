#pragma once

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

	//Pyramid* m_pPyramid;

	Entity m_pConnections[MAX_NBR_CONNECTION]; //0 :up-right, 1 : down-right, 2 : down -left, 3 : up- left
	//Qube* m_pEscheresqueRightConnections[MAX_NBR_CONNECTION];
	//Qube* m_pEscheresqueLeftConnections[MAX_NBR_CONNECTION];

	//ColoredDisk* m_pDiskConnection;

	Texture2D* const m_pDefaultText;
	Texture2D* const m_pIntermediateTexture;
	Texture2D* const m_pFlippedTexture;

	//Character* m_pCharacter;

	glm::vec2 m_CharacterPos{};
	//glm::vec2 m_EscheresqueLeftPos;
	//glm::vec2 m_EscheresqueRightPos;

	Level m_QubeLevel{ Level::Level1 };

	bool m_bIsFlipped{};
	bool m_bIsLastRow{};
	bool m_bIsSideColumn{};

	unsigned int m_JumpCounter{};

	void Serialize(StreamWriter& writer) const override;
};
