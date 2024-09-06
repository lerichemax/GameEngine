#include "PCH.h"

#include "QBertSystem.h"
#include "PyramidSystem.h"
#include "CharacterMovementSystem.h"
#include "JumperSystem.h"
#include "DiskSystem.h"

#include "AudioComponent.h"
#include "CharacterControllerComponent.h"
#include "QbertComponent.h"
#include "QubeComponent.h"
#include "MovementComponent.h"
#include "RendererComponent.h"
#include "CharacterLives.h"
#include "CharacterPoint.h"

#include "Coordinator.h"

void QBertSystem::SetSignature()
{
	Signature signature;
	signature.set(m_pRegistry->GetComponentType<CharacterControllerComponent>());
	signature.set(m_pRegistry->GetComponentType<QbertComponent>());
	signature.set(m_pRegistry->GetComponentType<MovementComponent>());

	m_pRegistry->SetSystemSignature<QBertSystem>(signature);
}

void QBertSystem::Serialize(StreamWriter& writer) const
{
	writer.WriteInt64("type", static_cast<int64>(std::type_index(typeid(QBertSystem)).hash_code()));
}

void QBertSystem::Start()
{
	Entity entity = *m_Entities.begin();

	auto* const pMoveComponent = m_pRegistry->GetComponent<MovementComponent>(entity);
	pMoveComponent->CurrentQube = GetSystem<PyramidSystem>()->GetTop();

	auto pQube = m_pRegistry->GetComponent<QubeComponent>(pMoveComponent->CurrentQube);

	auto pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
	pTransform->Translate(pQube->CharacterPos);

	auto* const pMoveSystem = GetSystem<CharacterMovementSystem>();

	pMoveSystem->OnMoveStarted.Subscribe([this](Entity entity) {
		if (!m_pRegistry->HasTag(entity, QBERT_TAG))
		{
			return;
		}
		auto* const pQbert = m_pRegistry->GetComponent<QbertComponent>(entity);
		pQbert->pJumpSound->Play();
	});

	auto* const pJumper = m_pRegistry->GetSystem<JumperSystem>();

	pJumper->OnJumpedToDeath.Subscribe([this](Entity entity) {
		if (!m_pRegistry->HasTag(entity, QBERT_TAG))
		{
			return;
		}

		auto* const pQbert = m_pRegistry->GetComponent<QbertComponent>(entity);
		pQbert->pFallSound->Play();

		m_pRegistry->GetComponent<RendererComponent>(entity)->Layer = 1;
		});

	pJumper->OnFell.Subscribe([this](Entity entity) {
		auto* const pLives = m_pRegistry->GetComponent<CharacterLives>(entity);
		if (IS_VALID(pLives))
		{
			pLives->Die();
			Reset(false, m_pRegistry->GetComponent<MovementComponent>(entity)->CurrentQube);
		}
	});

	m_pRegistry->GetSystem<DiskSystem>()->OnDiskReachedTop.Subscribe([this, entity](Entity diskEntity) {
		auto* pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);
		auto* const pMove = m_pRegistry->GetComponent<MovementComponent>(entity);

		pTransform->SetParent(nullptr);
		pMove->CurrentQube = m_pRegistry->GetSystem<PyramidSystem>()->GetTop();
		pMove->bCanMove = true;

		pTransform->Translate(m_pRegistry->GetComponent<QubeComponent>(pMove->CurrentQube)->CharacterPos);
		});
}

void QBertSystem::Reset(bool fullReset, Entity targetQubeEntity)
{
	Entity entity = *m_Entities.begin();

	auto* const pMovement = m_pRegistry->GetComponent<MovementComponent>(entity);

	pMovement->CurrentQube = targetQubeEntity;

	m_pRegistry->GetSystem<CharacterMovementSystem>()->MoveToCurrentQube(entity);

	m_pRegistry->SetEntityActive(entity, true);
	m_pRegistry->GetComponent<RendererComponent>(entity)->Layer = 8;

	if (!fullReset)
	{
		return;
	}
	//SwitchState(new OnQubeState(this, m_pJumper));

	auto* const pLives = m_pRegistry->GetComponent<CharacterLives>(entity);
	auto* const pPoints = m_pRegistry->GetComponent<CharacterPoint>(entity);
	pLives->Reset();
	pPoints->Reset();

}