#include "PCH.h"
#include "QubeSystem.h"

#include "QubeComponent.h"

#include "QBert.h"
#include "ColoredDisk.h"
#include "SlickSam.h"
#include "Texture2D.h"

#include "TextureRendererComponent.h"
#include "GameObject.h"
#include "PrefabsManager.h"
#include "ResourceManager.h"
#include "GameManager.h"

#include "Coordinator.h"

EventHandler<QubeSystem> QubeSystem::OnAnyQubeFlipped{};

void QubeSystem::Initialize()
{
	for (Entity entity : m_Entities)
	{
		auto* const pTextureRenderer = m_pCompManager->GetComponent<TextureRendererComponent>(entity);
		auto* const pQube = m_pCompManager->GetComponent<QubeComponent>(entity);
		auto* const pTransform = m_pCompManager->GetComponent<ECS_TransformComponent>(entity);

		pTextureRenderer->m_pTexture = pQube->pDefaultText;

		pQube->CharacterPos.x = pTransform->GetPosition().x + pTextureRenderer->m_pTexture->GetWidth() / 4;
		pQube->CharacterPos.y = pTransform->GetPosition().y - pTextureRenderer->m_pTexture->GetHeight() / 5;
	}

	//m_EscheresqueRightPos.x = GetGameObject()->GetECSTransform()->GetWorldPosition().x + GetGameObject()->GetComponent<RendererComponent>()->GetTextureWidth() * (3.f/5.f);
	//m_EscheresqueRightPos.y = GetGameObject()->GetECSTransform()->GetWorldPosition().y + GetGameObject()->GetComponent<RendererComponent>()->GetTextureHeight() / 2;

	//m_EscheresqueLeftPos.x = GetGameObject()->GetECSTransform()->GetWorldPosition().x - GetGameObject()->GetComponent<RendererComponent>()->GetTextureWidth() /5.f;
	//m_EscheresqueLeftPos.y = GetGameObject()->GetECSTransform()->GetWorldPosition().y + v->GetComponent<RendererComponent>()->GetTextureHeight() / 2;
}

void QubeSystem::Start()
{
}

void QubeSystem::Update()
{
	/*if (m_pDiskConnection != nullptr && m_pDiskConnection->HasQBert())
	{
		m_pDiskConnection = nullptr;
		m_pPyramid->DiskUsed();
	}*/
}

void QubeSystem::HandleQBertLanding()
{
	//switch (m_QubeLevel)
	//{
	//case Level::Level1:
	//	Flip();
	//	break;
	//case Level::Level2:
	//	if (m_JumpCounter < 1)
	//	{
	//		GetGameObject()->GetComponent<TextureRendererComponent>()->m_pTexture = m_pIntermediateTexture;
	//		m_JumpCounter++;
	//	}
	//	else 
	//	{
	//		Flip();
	//	}
	//	break;
	//case Level::Level3:
	//	if (IsFlipped())
	//	{
	//		UnFlip();
	//	}
	//	else {
	//		Flip();
	//	}
	//	break;
	//}
}

void QubeSystem::AddEscheresqueRightConnection(ConnectionDirection dir, QubeSystem* const pConnection)
{
	/*if (dir != ConnectionDirection::null)
	{
		m_pEscheresqueRightConnections[static_cast<int>(dir)] = pConnection;
	}*/	
}

void QubeSystem::AddEscheresqueLeftConnection(ConnectionDirection dir, QubeSystem* const pConnection)
{
	/*if (dir != ConnectionDirection::null)
	{
		m_pEscheresqueLeftConnections[static_cast<int>(dir)] = pConnection;
	}*/
}

void QubeSystem::AddConnectionToDisk()
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

bool QubeSystem::HasConnection(ConnectionDirection dir) const
{
	//if (dir == ConnectionDirection::null)
	//{
	//	return false;
	//}
	//return m_pConnections[static_cast<int>(dir)] != nullptr;
	return true;
}

bool QubeSystem::HasEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const
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

QubeSystem* const QubeSystem::GetEscheresqueConnection(ConnectionDirection dir, bool escheresqueRight) const
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

void QubeSystem::QBertJump(QBert* pQbert)
{
	//if (!m_bIsFlipped)
	//{
	//	pQbert->EarnPoints(POINTS_FOR_FLIP);
	//}
	
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

void QubeSystem::Flip()
{
	//if (!m_bIsFlipped)
	//{
	//	GetGameObject()->GetComponent<TextureRendererComponent>()->m_pTexture = m_pFlippedTexture;
	//	m_bIsFlipped = true;
	//	m_JumpCounter++;
	//	OnAnyQubeFlipped.Notify();
	//}
}

void QubeSystem::UnFlip()
{
	//m_bIsFlipped = false;
	//GetGameObject()->GetComponent<TextureRendererComponent>()->m_pTexture = m_pDefaultText;
	//m_JumpCounter=0;
}

void QubeSystem::Reset(Level level)
{
	//m_bIsFlipped = false;
	//m_JumpCounter = 0;
	//m_pCharacter = nullptr;
	//m_QubeLevel = level;
	
	//if (m_pGameObject->HasChildren())
	//{
	//	for(auto pChild : m_pGameObject->GetChildren())
	//	{
	//		pChild->Destroy();
	//	}
	//}
	//m_pDiskConnection = nullptr;
	
	//GetGameObject()->GetComponent<TextureRendererComponent>()->m_pTexture = m_pDefaultText;
}

ColoredDisk* QubeSystem::GetConnectedDisk() const
{
	//return m_pDiskConnection;
	return nullptr;
}

void QubeSystem::CharacterJumpIn(Character* pCharacter)
{
	//m_pCharacter = pCharacter;
	//if (typeid(*pCharacter) == typeid(SlickSam) && (m_bIsFlipped || m_JumpCounter > 0))
	//{
	//	UnFlip();
	//}
}

void QubeSystem::CharacterJumpOut()
{
	//m_pCharacter = nullptr;
}

void QubeSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteString("type", typeid(QubeSystem).name());
}

void QubeSystem::SetSignature(Coordinator* const pRegistry)
{
	Signature signature;
	signature.set(pRegistry->GetComponentType<QubeComponent>());
	signature.set(pRegistry->GetComponentType<ECS_TransformComponent>());
	signature.set(pRegistry->GetComponentType<TextureRendererComponent>());

	pRegistry->SetSystemSignature<QubeSystem>(signature);
}