#include "PCH.h"
#include "Qube.h"

#include "RendererComponent.h"
#include "GameObject.h"
#include "Subject.h"

#include "QubeObserver.h"
#include "DiskPrefab.h"
#include "ColoredDisk.h"
#include "SlickSam.h"

Qube::Qube(Texture2D* pDefText, Texture2D* pInterText, Texture2D* pFlippedText)
	:m_pDefaultText(pDefText),
	m_pIntermediateTexture(pInterText),
	m_pFlippedTexture(pFlippedText),
	m_pSubject(new Subject{}),
	m_pConnections{nullptr},
	m_CharacterPos()
{
}

void Qube::Initialize()
{
	m_CharacterPos.x =  m_pGameObject->GetTransform()->GetPosition().x + m_pGameObject->GetComponent<RendererComponent>()->GetTextureWidth() / 4;
	m_CharacterPos.y = m_pGameObject->GetTransform()->GetPosition().y - m_pGameObject->GetComponent<RendererComponent>()->GetTextureHeight() / 5;
}

Qube::~Qube()
{
	if (m_pSubject != nullptr)
	{
		delete m_pSubject;
	}
}

void Qube::Update()
{
	if (m_pDiskConnection != nullptr && m_pDiskConnection->HasQBert())
	{
		m_pDiskConnection = nullptr;
		m_pSubject->Notify(this, (int)QubeEvents::DiskUsed);
	}
}

void Qube::SetTexture(Texture2D* pText)
{
	m_pIntermediateTexture = pText;
	m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIntermediateTexture);
}

void Qube::AddConnection(ConnectionDirection dir, Qube* const pConnection)
{
	m_pConnections[(int)dir] = pConnection;
}

void Qube::AddConnectionToDisk(Qube* top)
{
	auto pDisk = new DiskPrefab{top};
	
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
	std::cout << "Disk spawned\n";
}

bool Qube::HasConnection(ConnectionDirection dir) const
{
	return m_pConnections[(int)dir] != nullptr;
}

void Qube::QBertJump()
{
	m_pSubject->Notify(this, (int)QubeEvents::PlayerJump);
}

void Qube::Flip()
{
	if (!m_bIsFlipped)
	{
		m_pSubject->Notify(this, (int)QubeEvents::QubeFlipped);
		m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pFlippedTexture);
		m_bIsFlipped = true;
		m_JumpCounter++;
	}
}

void Qube::IntermediateFlip()
{
	if (!m_bIsFlipped)
	{
		m_pSubject->Notify(this, (int)QubeEvents::QubeFlipped);
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