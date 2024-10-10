#pragma once
#include "System.h"

class Registry;
class CameraSystem : public System
{
public:
	void Update() override;

	bool TrySetMainCamera(Entity cameraEntity);

	void SetSignature() const override;

private:
	friend class Scene;

	Entity m_MainCameraEntity;
};