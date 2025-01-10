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

void TransformSystem::DecomposeMat3(glm::mat3 const& matrix, glm::vec2& translation, float& rotation, glm::vec2& scale)
{
	ExtractTranslation(matrix, translation);
	ExtractRotation(matrix, rotation);
	ExtractScale(matrix, scale);
}

void TransformSystem::ExtractTranslation(glm::mat3 const& matrix, glm::vec2& translation)
{
	translation = matrix[2];
}

void TransformSystem::ExtractRotation(glm::mat3 const& matrix, float& rotation)
{
	rotation = glm::atan(matrix[0][1], matrix[0][0]);
}

void TransformSystem::ExtractScale(glm::mat3 const& matrix, glm::vec2& scale)
{
	scale.x = glm::sqrt(glm::pow(matrix[0][0], 2.f) + glm::pow(matrix[0][1], 2.f));
	scale.y = glm::sqrt(glm::pow(matrix[1][0], 2.f) + glm::pow(matrix[1][1], 2.f));
}