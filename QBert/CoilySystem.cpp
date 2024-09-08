#include "PCH.h"
#include "CoilySystem.h"
#include "JumperSystem.h"
#include "PyramidSystem.h"

#include "AiControllerComponent.h"
#include "JumpComponent.h"
#include "MovementComponent.h"
#include "CoilyComponent.h"
#include "QbertComponent.h"

void CoilySystem::Start()
{
	m_pPyramid = m_pRegistry->GetSystem<PyramidSystem>();
	m_Qbert = FindComponentOfType<QbertComponent>()->GetGameObject()->GetEntity();

	m_pRegistry->GetSystem<JumperSystem>()->OnJumpedToDeath.Subscribe([this](Entity entity) {
		if (!m_pRegistry->HasTag(entity, ENEMY_TAG))
		{
			return;
		}
		auto* const pEnemy = m_pRegistry->GetComponent<AiControllerComponent>(entity);

		if (pEnemy->Type == EnemyType::Coily)
		{
			HandleCoilyTransform(entity);
		}
	});
}

void CoilySystem::HandleCoilyTransform(Entity entity)
{
	auto* const pJumper = m_pRegistry->GetComponent<JumpComponent>(entity);
	pJumper->bIsFalling = false;
	pJumper->bIsJumping = false;
	pJumper->bJumpDown = false;
	
	auto* const pMovement = m_pRegistry->GetComponent<MovementComponent>(entity);

	pMovement->SetTextureIdleNames("Textures/Enemies/Coily/Coily_Small_DownRight.png", 
		"Textures/Enemies/Coily/Coily_Small_DownLeft.png",
		"Textures/Enemies/Coily/Coily_Small_UpRight.png",
		"Textures/Enemies/Coily/Coily_Small_UpLeft.png");

	pMovement->SetTextureJumpNames("Textures/Enemies/Coily/Coily_Big_DownRight.png", 
		"Textures/Enemies/Coily/Coily_Big_DownLeft.png", 
		"Textures/Enemies/Coily/Coily_Big_UpRight.png", 
		"Textures/Enemies/Coily/Coily_Big_DownLeft.png");

	m_pRegistry->AddComponent<CoilyComponent>(entity);
}

void CoilySystem::ChooseDirection(MovementComponent* const pMover) const
{
	Entity coilyEntity = *m_Entities.begin();

	//auto* const pQbertMoveComp = m_pRegistry->GetComponent<MovementComponent>(m_Qbert);

	//int const currentIdx = m_pPyramid->GetQubeIndex(pMover->CurrentQube);
	//int const targetIdx = m_pPyramid->GetQubeIndex(pQbertMoveComp->CurrentQube);

	auto* const pCoily = m_pRegistry->GetComponent<CoilyComponent>(coilyEntity);
	
	pMover->CurrentDirection = pCoily->MovementQueue[pCoily->MOVEMENT_QUEUE_SIZE - pCoily->CurrentlyInQueue];
	pCoily->CurrentlyInQueue--;

}

void CoilySystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(CoilySystem)).hash_code()));
}

void CoilySystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<AiControllerComponent>());
	signature.set(m_pRegistry->GetComponentType<MovementComponent>());
	signature.set(m_pRegistry->GetComponentType<CoilyComponent>());
	m_pRegistry->SetSystemSignature<CoilySystem>(signature);
}