#include "PCH.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera2D::Camera2D(uint32_t width, uint32_t height)
	:m_Width{ width },
	m_Height{ height },
	m_Location{ },
	m_Scale{ 1.f },
	m_bNeedsUpdate{ true },
	m_CameraMatrix{ 1.f },
	m_OrthogonalProjection{ 1.f }
{
	m_OrthogonalProjection = glm::ortho( 0.f,
		static_cast<float>(m_Width),
		0.f,
		static_cast<float>(m_Height),
		-1.f,
		1.f
	);
}

void Camera2D::SetPosition(glm::vec2 const& position)
{
	m_Location = position;
	m_bNeedsUpdate = true;
}

void Camera2D::SetZoomLevel(float zoom)
{
	m_Scale = zoom;
	m_bNeedsUpdate = true;
}

glm::mat4 const& Camera2D::GetCameraMatrix() const
{
	return m_CameraMatrix;
}

void Camera2D::Update()
{
	if (!m_bNeedsUpdate)
	{
		return;
	}

	glm::vec3 translate{ -m_Location.x, -m_Location.y, 0 };
	m_CameraMatrix = glm::translate(m_OrthogonalProjection, translate );

	glm::vec3 scale{ m_Scale, m_Scale, 0 };
	m_CameraMatrix = glm::scale( m_OrthogonalProjection, scale );

	m_bNeedsUpdate = false;
}