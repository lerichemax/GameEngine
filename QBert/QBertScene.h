#pragma once
#include "Scene.h"

class QBertScene final : public empire::Scene
{
public:
	QBertScene();
	~QBertScene() = default;

	void Initialize() override;
};
