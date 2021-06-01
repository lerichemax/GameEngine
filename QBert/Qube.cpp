#include "PCH.h"
#include "Qube.h"

#include "RendererComponent.h"
#include "GameObject.h"
#include "QBert.h"

#include "QubeObserver.h"
#include "ColoredDisk.h"
#include "PrefabsManager.h"
#include "SlickSam.h"

Qube::Qube(Texture2D* pDefText, Texture2D* pInterText, Texture2D* pFlippedText)
	:m_pDefaultText(pDefText),
	m_pIntermediateTexture(pInterText),
	m_pFlippedTexture(pFlippedText),
	m_pConnections{nullptr},
	m_pCharacter(nullptr),
	m_bIsFlipped(false),
	m_JumpCounter(),
	m_pDiskConnection(nullptr),
	m_EscheresqueRightPos(),
	m_EscheresqueLeftPos(),
	m_CharacterPos(),
	m_bIsLastRow(false),
	m_bIsSideColumn(false),
	m_pEscheresqueRightConnections{},
	m_pEscheresqueLeftConnections{}

{
}

Qube::Qube(Qube const& other)
	:m_pDefaultText(other.m_pDefaultText),
	m_pIntermediateTexture(other.m_pIntermediateTexture),
	m_pFlippedTexture(other.m_pFlippedTexture),
	m_pConnections{  },
	m_pCharacter(other.m_pCharacter),
	m_bIsFlipped(other.m_bIsFlipped),
	m_JumpCounter(other.m_JumpCounter),
	m_pDiskConnection(other.m_pDiskConnection),
	m_EscheresqueRightPos(other.m_EscheresqueRightPos),
	m_EscheresqueLeftPos(other.m_EscheresqueLeftPos),
	m_CharacterPos(other.m_CharacterPos),
	m_bIsLastRow(other.m_bIsLastRow),
	m_bIsSideColumn(other.m_bIsSideColumn)

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
	m_CharacterPos.x =  m_pGameObject->GetTransform()->GetPosition().x + m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth() / 4;
	m_CharacterPos.y = m_pGameObject->GetTransform()->GetPosition().y - m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight() / 5;

	m_EscheresqueRightPos.x = m_pGameObject->GetTransform()->GetPosition().x + m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth() * (3.f/5.f);
	m_EscheresqueRightPos.y = m_pGameObject->GetTransform()->GetPosition().y + m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight() / 2;

	m_EscheresqueLeftPos.x = m_pGameObject->GetTransform()->GetPosition().x - m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth() /5.f;
	m_EscheresqueLeftPos.y = m_pGameObject->GetTransform()->GetPosition().y + m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight() / 2;
}

void Qube::Update()
{
	if (m_pDiskConnection != nullptr && m_pDiskConnection->HasQBert())
	{
		m_pDiskConnection = nullptr;
		m_pGameObject->Notify((int)QubeEvents::DiskUsed);
	}
}

void Qube::SetTexture(Texture2D* pText)
{
	m_pIntermediateTexture = pText;
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIntermediateTexture);
}

void Qube::AddConnection(ConnectionDirection dir, Qube* const pConnection)
{
	if (dir != ConnectionDirection::null)
	{
		m_pConnections[(int)dir] = pConnection;
	}
}

void Qube::AddEscheresqueRightConnection(ConnectionDirection dir, Qube* const pConnection)
{
	if (dir != ConnectionDirection::null)
	{
		m_pEscheresqueRightConnections[(int)dir] = pConnection;
	}	
}

void Qube::AddEscheresqueLeftConnection(ConnectionDirection dir, Qube* const pConnection)
{
	if (dir != ConnectionDirection::null)
	{
		m_pEscheresqueLeftConnections[(int)dir] = pConnection;
	}
}

void Qube::AddConnectionToDisk()
{
	auto pDisk = PrefabsManager::GetInstance().Instantiate("Disk");
	pDisk->GetComponent<ColoredDisk>()->SetPyramidTop(m_pGameObject->GetParent()->GetComponent<Pyramid>()->GetTop());
	
	auto parentPos = m_pGameObject->GetTransform()->GetPosition();
	if (!HasConnection(ConnectionDirection::upLeft))
	{
		pDisk->GetTransform()->Translate(parentPos.x - m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth()/2,
			parentPos.y - m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight()/3);
	}
	else
	{
		pDisk->GetTransform()->Translate(
			parentPos.x + m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth(),
			parentPos.y - m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight()/3);
	}
	
	m_pGameObject->AddChild(pDisk);
	
	m_pDiskConnection = pDisk->GetComponent<ColoredDisk>();
	Debugger::GetInstance().Log("Disk spawned");
}

bool Qube::HasConnection(ConnectionDirection dir) const
{
	return m_pConnections[(int)dir] != nullptr;
}

bool Qube::HasEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const
{
	if (escheresqueRight)
	{
		return m_pEscheresqueRightConnections[(int)dir] != nullptr;
	}
	else
	{
		return m_pEscheresqueLeftConnections[(int)dir] != nullptr;
	}
}

Qube* Qube::GetEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const
{
	if (escheresqueRight)
	{
		return m_pEscheresqueRightConnections[(int)dir];
	}
	else
	{
		return m_pEscheresqueLeftConnections[(int)dir];
	}
}

void Qube::QBertJump(QBert* pQbert)
{
	CharacterJumpIn(pQbert);
	m_pGameObject->Notify((int)QubeEvents::PlayerJump);
}

void Qube::Flip()
{
	if (!m_bIsFlipped)
	{
		m_pGameObject->Notify((int)QubeEvents::QubeFlipped);
		m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pFlippedTexture);
		m_bIsFlipped = true;
		m_JumpCounter++;
	}
}

void Qube::IntermediateFlip()
{
	if (!m_bIsFlipped)
	{
		m_pGameObject->Notify((int)QubeEvents::QubeFlipped);
		m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIntermediateTexture);
		m_JumpCounter++;
	}
}

void Qube::UnFlip()
{
	m_bIsFlipped = false;
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pDefaultText);
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
	
	GetGameObject()->GetComponent<RendererComponent>()->SetTexture(m_pDefaultText);
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