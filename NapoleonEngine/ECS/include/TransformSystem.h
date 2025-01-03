#pragma once
#include "System.h"

class TransformComponent;
class TransformSystem : public System {
public:
	void Update() override;

private:
	void RecursivelyUpdateHierarchy(TransformComponent* const transformComponent) const;
};