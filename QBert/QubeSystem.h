#pragma once
#include "System.h"
#include "Event.h"

#include "glm/glm.hpp"


class Character;
class ColoredDisk;
class QBertScene;
class PyramidSystem;
class QBert;
enum class ConnectionDirection;
enum class Level;
class QubeSystem final : public System 
{
public:
	QubeSystem() = default;
	~QubeSystem() = default;

	static EventHandler<QubeSystem> OnAnyQubeFlipped;
	
	//glm::vec2 GetEscheresqueRightPos() const { return m_EscheresqueRightPos; }
	//glm::vec2 GetEscheresqueLeftPos() const { return m_EscheresqueLeftPos; }
	
	//QubeSystem* const GetConnection(ConnectionDirection dir) const { return m_pConnections[(int)dir]; }
	QubeSystem* const GetEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const;
	ColoredDisk* GetConnectedDisk() const;
	
	bool HasConnection(ConnectionDirection dir) const;
	bool HasEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const;

	//bool HasConnectionToDisk() const { return m_pDiskConnection != nullptr; }
	//bool HasCharacter() const{ return m_pCharacter != nullptr;}

	//void SetPyramid(Pyramid* pPyramid) { m_pPyramid = pPyramid; }
	void HandleQBertLanding();

	void AddEscheresqueRightConnection(ConnectionDirection dir, QubeSystem* const pConnection);
	void AddEscheresqueLeftConnection(ConnectionDirection dir, QubeSystem* const pConnection);
	
	void AddConnectionToDisk();
	void QBertJump(QBert* pQube);
	void Reset(Level level);
	void CharacterJumpIn(Character* pCharacter);
	void CharacterJumpOut();

	void Serialize(StreamWriter& writer) const override;
	void SetSignature(Coordinator* const pRegistry) override;

protected:
	void Initialize() override;
	void Start() override;
	void Update() override;

private:


	void Flip();
	void UnFlip();
};