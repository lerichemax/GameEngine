#pragma once
#include "System.h"

class TransformSystem : public System {
public:
	void Update() override;

	void SetSignature() const override;

private:
	void RecursivelyUpdateHierarchy(TransformComponent* const transformComponent) const;
};