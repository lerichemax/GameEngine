#pragma once
#include "Component.h"

namespace empire
{
	class CameraComponent final : public Component
	{
	public:
		CameraComponent();
		CameraComponent* Clone() const override { return new CameraComponent(*this); }

		CameraComponent(CameraComponent&& other) = delete;
		CameraComponent& operator=(CameraComponent const& rhs) = delete;
		CameraComponent& operator=(CameraComponent&& rhs) = delete;
		~CameraComponent() = default;
		
		void Initialize() override;

		void Transform();
		glm::vec2 TransformIntoCameraSpace(glm::vec2 const pos);
		glm::mat3x3 GetCameraMatrix();
	private:
		unsigned int m_Width;
		unsigned int m_Height;

		void Update() override{};
		CameraComponent(CameraComponent const& other);
	};
}
