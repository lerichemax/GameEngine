#pragma once
#include "Component.h"
#include "Texture2D.h"
#include "glm/vec3.hpp"

using namespace empire;

enum class ConnectionDirection
{
	upRight,
	downRight,
	downLeft,
	upLeft
};

namespace empire
{
	class Subject;
}
class Qube final : public Component
{
public:
	Qube(Texture2D* pText, Texture2D* pFlippedText);
	bool IsFlipped() const { return m_bIsFlipped; }
	void SetTexture(Texture2D* pText);
	void AddConnection(ConnectionDirection dir, Qube* const pConnection);
	void SetQbertPos(glm::vec3 const& pos) { m_QbertPos = pos; };
	glm::vec3 GetQbertPos() const { return m_QbertPos; }
	bool HasConnection(ConnectionDirection dir) const;
	Qube* GetConnection(ConnectionDirection dir) const { return m_pConnections[(int)dir]; }
	void Flip();
	Subject* const GetSubject() const { return m_pSubject; }
private:
	unsigned int static const MAX_NBR_CONNECTION = 4;
	Qube* m_pConnections[MAX_NBR_CONNECTION]; //0 :up-right, 1 : down-right, 2 : down -left, 3 : up- left 
	
	Texture2D* m_pTexture;
	Texture2D* m_pFlippedTexture;
	glm::vec3 m_QbertPos;
	bool m_bIsFlipped;

	Subject* const m_pSubject;

	void Update() override {}
};