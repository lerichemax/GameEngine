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
	Qube() = default;
	explicit Qube(std::shared_ptr<Texture2D> pDefText, std::shared_ptr<Texture2D> pInterText, std::shared_ptr<Texture2D> pFlippedText);

	~Qube() = default;
	
	glm::vec2 GetCharacterPos() const { return m_CharacterPos; }
	//glm::vec2 GetEscheresqueRightPos() const { return m_EscheresqueRightPos; }
	//glm::vec2 GetEscheresqueLeftPos() const { return m_EscheresqueLeftPos; }
	
	//Qube* GetConnection(ConnectionDirection dir) const { return m_pConnections[(int)dir]; }
	std::shared_ptr<Qube> GetEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const;
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
	void SetTexture(std::shared_ptr<Texture2D> pText);
	//void SetPyramid(Pyramid* pPyramid) { m_pPyramid = pPyramid; }
	
	void AddConnection(ConnectionDirection dir, Qube* const pConnection);
	void AddEscheresqueRightConnection(ConnectionDirection dir, Qube* const pConnection);
	void AddEscheresqueLeftConnection(ConnectionDirection dir, Qube* const pConnection);
	
	void AddConnectionToDisk();
	void QBertJump(QBert* pQube);
	void Reset();
	void CharacterJumpIn(Character* pCharacter);
	void CharacterJumpOut();

	void Serialize(StreamWriter& writer) const override;
	void Deserialize(JsonReader const* reader, SerializationMap& context) override;

	void RestoreContext(JsonReader const* reader, SerializationMap const& context) override {};

protected:
	void Initialize() override;
	void Start() override;
	void Update() override;

private:
	int static const MAX_NBR_CONNECTION{ 4 };
	static int const POINTS_FOR_FLIP{ 25 };
	
	//QBertScene* m_pScene;
	//Pyramid* m_pPyramid;
	
	//Qube* m_pConnections[MAX_NBR_CONNECTION]; //0 :up-right, 1 : down-right, 2 : down -left, 3 : up- left
	//Qube* m_pEscheresqueRightConnections[MAX_NBR_CONNECTION];
	//Qube* m_pEscheresqueLeftConnections[MAX_NBR_CONNECTION];
	
	//ColoredDisk* m_pDiskConnection;
	
	std::shared_ptr<Texture2D> m_pDefaultText;
	std::shared_ptr<Texture2D> m_pIntermediateTexture;
	std::shared_ptr<Texture2D> m_pFlippedTexture;

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