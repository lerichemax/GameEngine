#include "PCH.h"
#include "TransformSystem.h"

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

void TransformSystem::Update()
{
	auto view = m_pRegistry->GetView<TransformComponent>();

	for (Entity entity : view)
	{
		auto* const pTrans = m_pRegistry->GetComponent<TransformComponent>(entity);

		if (!pTrans->IsActive())
		{
			continue;
		}

		pTrans->UpdateTransformHierarchy();
	}
}