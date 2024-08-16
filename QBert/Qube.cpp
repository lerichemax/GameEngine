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
#include "ResourceManager.h"
#include "GameManager.h"

EventHandler<Qube> Qube::OnAnyQubeFlipped{};

Qube::Qube()
	:m_pDefaultText(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube.png")),
	m_pIntermediateTexture(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Intermediate.png")),
	m_pFlippedTexture(ResourceManager::GetInstance().GetTexture("Textures/Qube/Qube_Flipped.png")),
	m_pCharacter(nullptr),
	m_CharacterPos(),
	m_QubeLevel{Level::Level1},
	m_bIsFlipped(false),
	m_bIsLastRow(false),
	m_bIsSideColumn(false),
	m_JumpCounter()
{
}

void Qube::Initialize()
{
	//m_pScene = dynamic_cast<QBertScene*>(m_pGameObject->GetParentScene());

	GetGameObject()->GetComponent<RendererComponent>()->m_pTexture = m_pDefaultText;

	m_CharacterPos.x = GetGameObject()->GetTransform()->GetPosition().x + GetGameObject()->GetComponent<RendererComponent>()->m_pTexture->GetWidth() / 4;
	m_CharacterPos.y = GetGameObject()->GetTransform()->GetPosition().y - GetGameObject()->GetComponent<RendererComponent>()->m_pTexture->GetHeight() / 5;

	//m_EscheresqueRightPos.x = GetGameObject()->GetECSTransform()->GetWorldPosition().x + GetGameObject()->GetComponent<RendererComponent>()->GetTextureWidth() * (3.f/5.f);
	//m_EscheresqueRightPos.y = GetGameObject()->GetECSTransform()->GetWorldPosition().y + GetGameObject()->GetComponent<RendererComponent>()->GetTextureHeight() / 2;

	//m_EscheresqueLeftPos.x = GetGameObject()->GetECSTransform()->GetWorldPosition().x - GetGameObject()->GetComponent<RendererComponent>()->GetTextureWidth() /5.f;
	//m_EscheresqueLeftPos.y = GetGameObject()->GetECSTransform()->GetWorldPosition().y + v->GetComponent<RendererComponent>()->GetTextureHeight() / 2;
}

void Qube::Start()
{
}

void Qube::Update()
{
	/*if (m_pDiskConnection != nullptr && m_pDiskConnection->HasQBert())
	{
		m_pDiskConnection = nullptr;
		m_pPyramid->DiskUsed();
	}*/
}

void Qube::SetTexture(std::shared_ptr<Texture2D> pText)
{
	m_pIntermediateTexture = pText;
	//m_pGameObject->GetComponent<RendererComponent>()->SetTexture(m_pIntermediateTexture);
}

void Qube::HandleQBertLanding()
{
	switch (m_QubeLevel)
	{
	case Level::Level1:
		Flip();
		break;
	case Level::Level2:
		if (m_JumpCounter < 1)
		{
			GetGameObject()->GetComponent<RendererComponent>()->m_pTexture = m_pIntermediateTexture;
			m_JumpCounter++;
		}
		else 
		{
			Flip();
		}
		break;
	case Level::Level3:
		if (IsFlipped())
		{
			UnFlip();
		}
		else {
			Flip();
		}
		break;
	}
}

void Qube::AddConnection(ConnectionDirection dir, std::shared_ptr<Qube> pConnection)
{
	if (dir != ConnectionDirection::null)
	{
		m_pConnections[static_cast<int>(dir)] = pConnection;
	}
}

void Qube::AddEscheresqueRightConnection(ConnectionDirection dir, Qube* const pConnection)
{
	/*if (dir != ConnectionDirection::null)
	{
		m_pEscheresqueRightConnections[static_cast<int>(dir)] = pConnection;
	}*/	
}

void Qube::AddEscheresqueLeftConnection(ConnectionDirection dir, Qube* const pConnection)
{
	/*if (dir != ConnectionDirection::null)
	{
		m_pEscheresqueLeftConnections[static_cast<int>(dir)] = pConnection;
	}*/
}

void Qube::AddConnectionToDisk()
{
	//auto const pDisk = PrefabsManager::GetInstance().Instantiate("Disk");
	//pDisk->GetComponent<ColoredDisk>()->SetPyramidTop(m_pGameObject->GetParent()->GetComponent<Pyramid>()->GetTop());

	//m_pGameObject->AddChild(pDisk);
	//auto const parentPos = m_pGameObject->GetTransform()->GetWorldPosition();
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
	/*if (escheresqueRight)
	{
		return m_pEscheresqueRightConnections[static_cast<int>(dir)] != nullptr;
	}
	else
	{
		return m_pEscheresqueLeftConnections[static_cast<int>(dir)] != nullptr;
	}*/
	return true;
}

std::shared_ptr<Qube> Qube::GetEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const
{

	//if (escheresqueRight)
	//{
	//	return m_pEscheresqueRightConnections[static_cast<int>(dir)];
	//}
	//else
	//{
	//	return m_pEscheresqueLeftConnections[static_cast<int>(dir)];
	//}
	return nullptr;
}

void Qube::QBertJump(QBert* pQbert)
{
	if (!m_bIsFlipped)
	{
		pQbert->EarnPoints(POINTS_FOR_FLIP);
	}
	
	//switch (m_pScene->GetLevel())
	//{
	//case QBertScene::Level::Level1:
	//	Flip();
	//	if (m_pPyramid->AreAllQubesFlipped())
	//	{
	//		Debugger::GetInstance().Log("YOU FINISHED LEVEL 1!");
	//		m_pScene->ResetScene(QBertScene::Level::Level2);
	//	}
	//	break;
	//case QBertScene::Level::Level2:
	//	if (m_JumpCounter == 0)
	//	{
	//		IntermediateFlip();
	//		return;
	//	}
	//	else if (m_JumpCounter == 1)
	//	{
	//		Flip();
	//		if (m_pPyramid->AreAllQubesFlipped())
	//		{
	//			Debugger::GetInstance().Log("YOU FINISHED LEVEL 2!");
	//			m_pScene->ResetScene(QBertScene::Level::Level3);
	//		}
	//	}
	//	break;
	//case QBertScene::Level::Level3:
	//	if (m_bIsFlipped)
	//	{
	//		UnFlip();
	//	}
	//	else
	//	{
	//		Flip();
	//	}
	//	
	//	if (m_pPyramid->AreAllQubesFlipped())
	//	{
	//		Debugger::GetInstance().Log("YOU FINISHED LEVEL 3!");
	//		m_pScene->EndGame(true);
	//	}
	//	break;
	//}
	
}

void Qube::Flip()
{
	if (!m_bIsFlipped)
	{
		GetGameObject()->GetComponent<RendererComponent>()->m_pTexture = m_pFlippedTexture;
		m_bIsFlipped = true;
		m_JumpCounter++;
		OnAnyQubeFlipped.Notify();
	}
}

void Qube::UnFlip()
{
	m_bIsFlipped = false;
	GetGameObject()->GetComponent<RendererComponent>()->m_pTexture = m_pDefaultText;
	m_JumpCounter=0;
}

void Qube::Reset(Level level)
{
	m_bIsFlipped = false;
	m_JumpCounter = 0;
	m_pCharacter = nullptr;
	m_QubeLevel = level;
	
	//if (m_pGameObject->HasChildren())
	//{
	//	for(auto pChild : m_pGameObject->GetChildren())
	//	{
	//		pChild->Destroy();
	//	}
	//}
	//m_pDiskConnection = nullptr;
	
	GetGameObject()->GetComponent<RendererComponent>()->m_pTexture = m_pDefaultText;
}

ColoredDisk* Qube::GetConnectedDisk() const
{
	//return m_pDiskConnection;
	return nullptr;
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

void Qube::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(Qube).name());

	BehaviourComponent::Serialize(writer);
}