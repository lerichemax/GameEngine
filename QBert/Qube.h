#pragma once
#include "BehaviourComponent.h"

#include "glm/glm.hpp"

enum class ConnectionDirection
{
	upRight,
	downRight,
	downLeft,
	upLeft,
	null
};


class Subject;
class Character;
class ColoredDisk;
class QBertScene;
class Pyramid;
class QBert;
class Qube final : public BehaviourComponent 
{
public:
	explicit Qube(Texture2D* pDefText, Texture2D* pInterText, Texture2D* pFlippedText);
	
	Qube(Qube&& other) = delete;
	Qube& operator=(Qube const& rhs) = delete;
	Qube& operator=(Qube&& rhs) = delete;
	~Qube() = default;

	void Initialize() override;
	void Update() override;
	
	glm::vec2 GetCharacterPos() const { return m_CharacterPos; }
	//glm::vec2 GetEscheresqueRightPos() const { return m_EscheresqueRightPos; }
	//glm::vec2 GetEscheresqueLeftPos() const { return m_EscheresqueLeftPos; }
	
	//Qube* GetConnection(ConnectionDirection dir) const { return m_pConnections[(int)dir]; }
	Qube* GetEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const;
	unsigned int GetJumpCounter() const { return m_JumpCounter; }
	ColoredDisk* GetConnectedDisk() const;
	Character* GetCharacter() const { return m_pCharacter; }
	
	bool HasConnection(ConnectionDirection dir) const;
	bool HasEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const;
	bool IsFlipped() const { return m_bIsFlipped; }
	//bool HasConnectionToDisk() const { return m_pDiskConnection != nullptr; }
	bool HasCharacter() const{ return m_pCharacter != nullptr;}
	bool IsLastRow() const { return m_bIsLastRow; }
	bool IsSideColumn()const { return m_bIsSideColumn; }
	
	void SetIsLastRow(bool lastRow) { m_bIsLastRow = lastRow; };
	void SetIsSideColumn(bool sideCol) { m_bIsSideColumn = sideCol; }
	void SetTexture(Texture2D* pText);
	//void SetPyramid(Pyramid* pPyramid) { m_pPyramid = pPyramid; }
	
	void AddConnection(ConnectionDirection dir, Qube* const pConnection);
	void AddEscheresqueRightConnection(ConnectionDirection dir, Qube* const pConnection);
	void AddEscheresqueLeftConnection(ConnectionDirection dir, Qube* const pConnection);
	
	void AddConnectionToDisk();
	void QBertJump(QBert* pQube);
	void Reset();
	void CharacterJumpIn(Character* pCharacter);
	void CharacterJumpOut();

private:
	int static const MAX_NBR_CONNECTION{ 4 };
	static int const POINTS_FOR_FLIP{ 25 };
	
	//QBertScene* m_pScene;
	//Pyramid* m_pPyramid;
	
	//Qube* m_pConnections[MAX_NBR_CONNECTION]; //0 :up-right, 1 : down-right, 2 : down -left, 3 : up- left
	//Qube* m_pEscheresqueRightConnections[MAX_NBR_CONNECTION];
	//Qube* m_pEscheresqueLeftConnections[MAX_NBR_CONNECTION];
	
	//ColoredDisk* m_pDiskConnection;
	
	Texture2D* m_pDefaultText;
	Texture2D* m_pIntermediateTexture;
	Texture2D* m_pFlippedTexture;

	Character* m_pCharacter;
	
	glm::vec2 m_CharacterPos;
	//glm::vec2 m_EscheresqueLeftPos;
	//glm::vec2 m_EscheresqueRightPos;
	
	bool m_bIsFlipped;
	bool m_bIsLastRow;
	bool m_bIsSideColumn;
	
	unsigned int m_JumpCounter;

	void Flip();
	void IntermediateFlip();
	void UnFlip();
};