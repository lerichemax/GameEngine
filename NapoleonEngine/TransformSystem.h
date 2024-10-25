#pragma once
#include "System.h"

class TransformComponent;
class TransformSystem : public System {
public:
	void Update() override;

	void SetSignature() const override;

private:
	void RecursivelyUpdateHierarchy(TransformComponent* const transformComponent) const;
	void BuildWorldTransformMatrix(TransformComponent* const pTransform);
};