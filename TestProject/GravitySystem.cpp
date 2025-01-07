#include "PCH.h"
#include "GravitySystem.h"

#include "RotationComponent.h"

void GravitySystem::Update()
{
	auto view = m_pRegistry->GetView<TransformComponent, RotationComponent>();

	for (Entity const& entity : view)
	{
		auto pTransform = m_pRegistry->GetComponent<TransformComponent>(entity);

		if (!IS_VALID(pTransform->m_pParent))
		{
			continue;
		}

		auto pRotator = m_pRegistry->GetComponent<RotationComponent>(entity);

		float distance = glm::distance(pTransform->GetLocation(), pTransform->m_pParent->GetLocation());

		glm::vec2 loc = pTransform->m_pParent->GetLocation();

		loc.x += std::cos(pRotator->Angle) * distance;
		loc.y += std::sin(pRotator->Angle) * distance;

		pTransform->SetLocation(loc);

		pRotator->Angle += pRotator->Speed * TimerLocator::Get()->GetDeltaTime();
	}
}