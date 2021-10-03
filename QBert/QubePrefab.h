#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>

class QubePrefab final : public empire::GameObject
{
public:
	QubePrefab(glm::vec2 const& pos);

};