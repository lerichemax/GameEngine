#pragma once
#include "Component.h"

namespace empire
{
	class CameraComponent final : public Component
	{
	public:
		CameraComponent();
		CameraComponent(CameraComponent&& other) = delete;
		CameraComponent& operator=(CameraComponent const& rhs) = delete;
		CameraComponent& operator=(CameraComponent&& rhs) = delete;
		~CameraComponent() = default;


		
		glm::vec2 TransformIntoCameraSpace(glm::vec2 const pos);
		glm::mat3x3 GetCameraMatrix() const;
	
	protected:
		void Initialize() override;
		void Update() override {};

		CameraComponent* Clone() const override { return new CameraComponent(*this); }
	
	private:
		friend class Scene;
		
		unsigned int m_Width;
		unsigned int m_Height;

		void Transform() const;
		
		CameraComponent(CameraComponent const& other);
	};
}
