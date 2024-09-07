#include "PCH.h"
#include "QubeSystem.h"
#include "JumperSystem.h"

#include "QubeComponent.h"
#include "MovementComponent.h"
#include "QbertComponent.h"
#include "AiControllerComponent.h"

#include "QBert.h"
#include "SlickSam.h"
#include "Texture2D.h"

#include "RendererComponent.h"
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
		auto* const pTextureRenderer = m_pRegistry->GetComponent<RendererComponent>(entity);
		auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(entity);
		auto* const pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

		pTextureRenderer->pTexture = pQube->pDefaultText;

		pQube->CharacterPos.x = pTransform->GetLocation().x + pTextureRenderer->pTexture->GetWidth() / 4;
		pQube->CharacterPos.y = pTransform->GetLocation().y - pTextureRenderer->pTexture->GetHeight() / 5;
	}

	//m_EscheresqueRightPos.x = GetGameObject()->GetECSTransform()->GetWorldLocation().x + GetGameObject()->GetComponent<RendererComponent>()->GetTextureWidth() * (3.f/5.f);
	//m_EscheresqueRightPos.y = GetGameObject()->GetECSTransform()->GetWorldLocation().y + GetGameObject()->GetComponent<RendererComponent>()->GetTextureHeight() / 2;

	//m_EscheresqueLeftPos.x = GetGameObject()->GetECSTransform()->GetWorldLocation().x - GetGameObject()->GetComponent<RendererComponent>()->GetTextureWidth() /5.f;
	//m_EscheresqueLeftPos.y = GetGameObject()->GetECSTransform()->GetWorldLocation().y + v->GetComponent<RendererComponent>()->GetTextureHeight() / 2;
}

void QubeSystem::Start()
{
	auto* const pJumpSystem = GetSystem<JumperSystem>();

	if (pJumpSystem != nullptr)
	{
		pJumpSystem->OnJumpLanded.Subscribe([this](Entity entity) {
			
			auto* const pMoveComp = m_pRegistry->GetComponent<MovementComponent>(entity);
			auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComp->CurrentQube);

			pQube->bIsOccupied = true;

			if (m_pRegistry->HasTag(entity, QBERT_TAG))
			{
				HandleQBertLanding(pMoveComp->CurrentQube);
			}

			if (m_pRegistry->HasTag(entity, ENEMY_TAG))
			{
				auto* const pEnemy = m_pRegistry->GetComponent<AiControllerComponent>(entity);
				if (pEnemy->Type == EnemyType::SlickSam && pQube->bIsFlipped)
				{
					UnFlip(pMoveComp->CurrentQube);
				}
			}
		});
	}
}

void QubeSystem::Update()
{
	/*if (m_pDiskConnection != nullptr && m_pDiskConnection->HasQBert())
	{
		m_pDiskConnection = nullptr;
		m_pPyramid->DiskUsed();
	}*/
}

void QubeSystem::HandleQBertLanding(Entity qubeEntity)
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);
	switch (pQube->QubeLevel)
	{
	case Level::Level1:
		Flip(qubeEntity);
		break;
	case Level::Level2:
		if (pQube->JumpCounter < 1)
		{
			auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(qubeEntity);

			pRenderer->pTexture = pQube->pIntermediateTexture;
			pQube->JumpCounter++;
		}
		else 
		{
			Flip(qubeEntity);
		}
		break;
	case Level::Level3:
		if (pQube->bIsFlipped)
		{
			UnFlip(qubeEntity);
		}
		else {
			Flip(qubeEntity);
		}
		break;
	}
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

void QubeSystem::Flip(Entity qubeEntity)
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);
	auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(qubeEntity);

	if (!pQube->bIsFlipped)
	{
		pRenderer->pTexture = pQube->pFlippedTexture;
		pQube->bIsFlipped = true;
		pQube->JumpCounter++;
		OnAnyQubeFlipped.Notify();
	}
}

void QubeSystem::UnFlip(Entity qubeEntity)
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(qubeEntity);
	auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(qubeEntity);

	pQube->bIsFlipped = false;
	pRenderer->pTexture = pQube->pDefaultText;
	pQube->JumpCounter=0;
}

void QubeSystem::Reset(Level level, Entity entity)
{
	auto* const pQube = m_pRegistry->GetComponent<QubeComponent>(entity);

	pQube->bIsFlipped = false;
	pQube->JumpCounter = 0;
	//m_pCharacter = nullptr;
	pQube->QubeLevel = level;
	
	//if (m_pGameObject->HasChildren())
	//{
	//	for(auto pChild : m_pGameObject->GetChildren())
	//	{
	//		pChild->Destroy();
	//	}
	//}
	//m_pDiskConnection = nullptr;

	auto* const pRenderer = m_pRegistry->GetComponent<RendererComponent>(entity);
	
	pRenderer->pTexture = pQube->pDefaultText;
}

DiskSystem* QubeSystem::GetConnectedDisk() const
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
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(QubeSystem)).hash_code()));
}

void QubeSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<QubeComponent>());
	signature.set(m_pRegistry->GetComponentType<TransformComponent>());
	signature.set(m_pRegistry->GetComponentType<RendererComponent>());

	m_pRegistry->SetSystemSignature<QubeSystem>(signature);
}
