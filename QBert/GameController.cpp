#include "PCH.h"
#include "GameController.h"
#include "GameObject.h"
#include "QubePrefab.h"
#include "TransformComponent.h"
#include "RendererComponent.h"
#include "Qube.h"
#include "GameObserver.h"
#include "Subject.h"

GameController::GameController(unsigned int maxWidth)
	:m_MaxWidth(maxWidth)
{
}

void GameController::Init(empire::GameObject* pParent)
{
	Component::Init(pParent);

	glm::vec3 startPos{m_pParentObject->GetTransform()->GetPosition()};
	glm::vec3 lastPos{startPos};

	auto observer = new GameObserver{ this };
	//spawn qubes
	for (unsigned int i = m_MaxWidth; i != 0; i--)
	{
		lastPos = startPos;
		for (unsigned int j = 0; j < i; j++)
		{
			auto pQube = new QubePrefab{};
			m_pQubes.push_back(pQube->GetComponent<Qube>());
			m_pQubes.back()->GetSubject()->AddObserver(observer);
			pQube->GetTransform()->Translate(lastPos);

			auto pos = pQube->GetTransform()->GetPosition();
			pos.x += pQube->GetComponent<RendererComponent>()->GetTextureWidth() / 4;
			pos.y -= pQube->GetComponent<RendererComponent>()->GetTextureHeight() / 4;
			pQube->GetComponent<Qube>()->SetQbertPos(pos);
			
			lastPos.x += pQube->GetComponent<RendererComponent>()->GetTextureWidth();
			m_pParentObject->AddChild(pQube);
		}
		startPos.x += m_pQubes.back()->GetGameObject()->GetComponent<RendererComponent>()->GetTextureWidth() / 2;
		startPos.y -= m_pQubes.back()->GetGameObject()->GetComponent<RendererComponent>()->GetTextureHeight()*0.75f;
	}
	std::reverse(m_pQubes.begin(), m_pQubes.end());

	CreateConnections();
}

void GameController::CreateConnections()
{
	int leftChild{};
	int rightChild{};
	int offset{ 1 };
	int endOfLine{ 1 };
	int counter{ 0 };
	
	for (int i = 0; i < (int)m_pQubes.size(); i++)
	{
		rightChild = i * 2 + offset;
		leftChild = i * 2 + offset+1;
		counter++;
		
		if (counter != endOfLine)
		{	
			offset--;
		}
		else
		{
			counter = 0;
			endOfLine++;
		}
		
		if (leftChild < m_pQubes.size())
		{
			m_pQubes[i]->AddConnection(ConnectionDirection::downLeft, m_pQubes[leftChild]);
			m_pQubes[leftChild]->AddConnection(ConnectionDirection::upRight, m_pQubes[i]);
		}
		if (rightChild < m_pQubes.size())
		{
			m_pQubes[i]->AddConnection(ConnectionDirection::downRight, m_pQubes[rightChild]);
			m_pQubes[rightChild]->AddConnection(ConnectionDirection::upLeft, m_pQubes[i]);
		}
	}
}

bool GameController::AreAllQubesFlipped() const
{
	for (auto pQube : m_pQubes)
	{
		if (!pQube->IsFlipped())
		{
			return false;
		}
	}
	return true;
}