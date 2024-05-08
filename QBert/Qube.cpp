#include "PCH.h"
#include "Qube.h"

#include "QBertScene.h"
#include "QBert.h"
#include "ColoredDisk.h"
#include "SlickSam.h"
#include "Pyramid.h"

#include "RendererComponent.h"
#include "GameObject.h"
#include "PrefabsManager.h"


Qube::Qube(Texture2D* pDefText, Texture2D* pInterText, Texture2D* pFlippedText)
	:m_pScene(nullptr),
	m_pPyramid(nullptr),
	m_pConnections{ nullptr },
	m_pEscheresqueRightConnections{nullptr},
	m_pEscheresqueLeftConnections{nullptr},
	m_pDiskConnection(nullptr),
	m_pDefaultText(pDefText),
	m_pIntermediateTexture(pInterText),
	m_pFlippedTexture(pFlippedText),
	m_pCharacter(nullptr),
	m_CharacterPos(),
	m_EscheresqueLeftPos(),
	m_EscheresqueRightPos(),
	m_bIsFlipped(false),
	m_bIsLastRow(false),
	m_bIsSideColumn(false),
	m_JumpCounter()
{
}

Qube::Qube(Qube const& other)
	:m_pScene(other.m_pScene),
	m_pPyramid(other.m_pPyramid),
	m_pConnections{nullptr},
	m_pEscheresqueLeftConnections{nullptr},
	m_pEscheresqueRightConnections{nullptr},
	m_pDiskConnection(other.m_pDiskConnection),
	m_pDefaultText(other.m_pDefaultText),
	m_pIntermediateTexture(other.m_pIntermediateTexture),
	m_pFlippedTexture(other.m_pFlippedTexture),
	m_pCharacter(other.m_pCharacter),
	m_CharacterPos(other.m_CharacterPos),
	m_EscheresqueLeftPos(other.m_EscheresqueLeftPos),
	m_EscheresqueRightPos(other.m_EscheresqueRightPos),
	m_bIsFlipped(other.m_bIsFlipped),
	m_bIsLastRow(other.m_bIsLastRow),
	m_bIsSideColumn(other.m_bIsSideColumn),
	m_JumpCounter(other.m_JumpCounter)
{
	for (unsigned int i{}; i <MAX_NBR_CONNECTION;++i)
	{
		m_pConnections[i] = other.m_pConnections[i];
		m_pEscheresqueLeftConnections[i] = other.m_pEscheresqueLeftConnections[i];
		m_pEscheresqueRightConnections[i] = other.m_pEscheresqueRightConnections[i];
	}
}

void Qube::Initialize()
{
	m_pScene = dynamic_cast<QBertScene*>(m_pGameObject->GetParentScene());
	
	//m_CharacterPos.x =  m_pGameObject->GetECSTransform()->GetWorldPosition().x + m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth() / 4;
	//m_CharacterPos.y = m_pGameObject->GetECSTransform()->GetWorldPosition().y - m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight() / 5;

	//m_EscheresqueRightPos.x = m_pGameObject->GetECSTransform()->GetWorldPosition().x + m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth() * (3.f/5.f);
	//m_EscheresqueRightPos.y = m_pGameObject->GetECSTransform()->GetWorldPosition().y + m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight() / 2;

	//m_EscheresqueLeftPos.x = m_pGameObject->GetECSTransform()->GetWorldPosition().x - m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth() /5.f;
	//m_EscheresqueLeftPos.y = m_pGameObject->GetECSTransform()->GetWorldPosition().y + m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight() / 2;
}

void Qube::Update()
{
	if (m_pDiskConnection != nullptr && m_pDiskConnection->HasQBert())
	{
		m_pDiskConnection = nullptr;
		m_pPyramid->DiskUsed();
	}
}

void Qube::SetTexture(Texture2D* pText)
{
	m_pIntermediateTexture = pText;
	//m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIntermediateTexture);
}

void Qube::AddConnection(ConnectionDirection dir, Qube* const pConnection)
{
	if (dir != ConnectionDirection::null)
	{
		m_pConnections[static_cast<int>(dir)] = pConnection;
	}
}

void Qube::AddEscheresqueRightConnection(ConnectionDirection dir, Qube* const pConnection)
{
	if (dir != ConnectionDirection::null)
	{
		m_pEscheresqueRightConnections[static_cast<int>(dir)] = pConnection;
	}	
}

void Qube::AddEscheresqueLeftConnection(ConnectionDirection dir, Qube* const pConnection)
{
	if (dir != ConnectionDirection::null)
	{
		m_pEscheresqueLeftConnections[static_cast<int>(dir)] = pConnection;
	}
}

void Qube::AddConnectionToDisk()
{
	auto const pDisk = PrefabsManager::GetInstance().Instantiate("Disk");
	//pDisk->GetComponent<ColoredDisk>()->SetPyramidTop(m_pGameObject->GetParent()->GetComponent<Pyramid>()->GetTop());

	m_pGameObject->AddChild(pDisk);
	auto const parentPos = m_pGameObject->GetECSTransform()->GetWorldPosition();
	if (!HasConnection(ConnectionDirection::upLeft))
	{
		/*pDisk->GetECSTransform()->SetWorldPosition(parentPos.x - m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth()/2,
			parentPos.y - m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight()/3);*/
		/*pDisk->GetECSTransform()->Translate(parentPos.x - m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth()/2,
			parentPos.y - m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight()/3);*/
	}
	else
	{
		/*pDisk->GetECSTransform()->SetWorldPosition(
			parentPos.x + m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth(),
			parentPos.y - m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight()/3);*/
		/*pDisk->GetECSTransform()->Translate(
			parentPos.x + m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth(),
			parentPos.y - m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight()/3);*/
	}
	
	//m_pDiskConnection = pDisk->GetComponent<ColoredDisk>();
	Debugger::GetInstance().Log("Disk spawned");
}

bool Qube::HasConnection(ConnectionDirection dir) const
{
	if (dir == ConnectionDirection::null)
	{
		return false;
	}
	return m_pConnections[static_cast<int>(dir)] != nullptr;
}

bool Qube::HasEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const
{
	if (escheresqueRight)
	{
		return m_pEscheresqueRightConnections[static_cast<int>(dir)] != nullptr;
	}
	else
	{
		return m_pEscheresqueLeftConnections[static_cast<int>(dir)] != nullptr;
	}
}

Qube* Qube::GetEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const
{
	if (escheresqueRight)
	{
		return m_pEscheresqueRightConnections[static_cast<int>(dir)];
	}
	else
	{
		return m_pEscheresqueLeftConnections[static_cast<int>(dir)];
	}
}

void Qube::QBertJump(QBert* pQbert)
{
	if (!m_bIsFlipped)
	{
		pQbert->EarnPoints(POINTS_FOR_FLIP);
	}
	
	switch (m_pScene->GetLevel())
	{
	case QBertScene::Level::Level1:
		Flip();
		if (m_pPyramid->AreAllQubesFlipped())
		{
			Debugger::GetInstance().Log("YOU FINISHED LEVEL 1!");
			m_pScene->ResetScene(QBertScene::Level::Level2);
		}
		break;
	case QBertScene::Level::Level2:
		if (m_JumpCounter == 0)
		{
			IntermediateFlip();
			return;
		}
		else if (m_JumpCounter == 1)
		{
			Flip();
			if (m_pPyramid->AreAllQubesFlipped())
			{
				Debugger::GetInstance().Log("YOU FINISHED LEVEL 2!");
				m_pScene->ResetScene(QBertScene::Level::Level3);
			}
		}
		break;
	case QBertScene::Level::Level3:
		if (m_bIsFlipped)
		{
			UnFlip();
		}
		else
		{
			Flip();
		}
		
		if (m_pPyramid->AreAllQubesFlipped())
		{
			Debugger::GetInstance().Log("YOU FINISHED LEVEL 3!");
			m_pScene->EndGame(true);
		}
		break;
	}
	
}

void Qube::Flip()
{
	if (!m_bIsFlipped)
	{
		//m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pFlippedTexture);
		m_bIsFlipped = true;
		m_JumpCounter++;
	}
}

void Qube::IntermediateFlip()
{
	if (!m_bIsFlipped)
	{
		//m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIntermediateTexture);
		m_JumpCounter++;
	}
}

void Qube::UnFlip()
{
	m_bIsFlipped = false;
	//m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pDefaultText);
	m_JumpCounter=0;
}

void Qube::Reset()
{
	m_bIsFlipped = false;
	m_JumpCounter = 0;
	m_pCharacter = nullptr;
	
	if (m_pGameObject->HasChildren())
	{
		for(auto pChild : m_pGameObject->GetChildren())
		{
			pChild->Destroy();
		}
	}
	m_pDiskConnection = nullptr;
	
	//GetGameObject()->GetComponent<RendererComponent>()->SetTexture(m_pDefaultText);
}

ColoredDisk* Qube::GetConnectedDisk() const
{
	return m_pDiskConnection;
}

void Qube::CharacterJumpIn(Character* pCharacter)
{
	m_pCharacter = pCharacter;
	if (typeid(*pCharacter) == typeid(SlickSam) && (m_bIsFlipped || m_JumpCounter > 0))
	{
		UnFlip();
	}
}

void Qube::CharacterJumpOut()
{
	m_pCharacter = nullptr;
}