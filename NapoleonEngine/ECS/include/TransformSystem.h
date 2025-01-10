#pragma once
#include "System.h"

class TransformComponent;
class TransformSystem : public System {
public:
	void Update() override;

	static void DecomposeMat3(glm::mat3 const& matrix, glm::vec2& translation, float& rotation, glm::vec2& scale); //TODO : angle to quaternion or rotation vector
	static void ExtractTranslation(glm::mat3 const& matrix, glm::vec2& translation);
	static void ExtractRotation(glm::mat3 const& matrix, float& rotation);
	static void ExtractScale(glm::mat3 const& matrix, glm::vec2& scale);

};