#include "PCH.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera2D::Camera2D(uint32_t width, uint32_t height)
	:m_Width{ width },
	m_Height{ height },
	m_Location{ -(width / 2.f), -(height / 2.f) },
	m_Scale{ 1.f },
	m_bNeedsUpdate{ false },
	m_CameraMatrix{ 1.f }
	//m_OrthogonalProjection{ 1.f }
{
	//m_OrthogonalProjection = glm::ortho( 0.f,
	//	static_cast<float>(m_Width),
	//	0.f,
	//	static_cast<float>(m_Height),
	//	-1.f,
	//	1.f
	//);

	m_CameraMatrix = glm::translate(m_CameraMatrix, -m_Location);
	glm::vec2 scale{ m_Scale, m_Scale};
	m_CameraMatrix = glm::scale(m_CameraMatrix, scale);
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

glm::mat3 const& Camera2D::GetCameraMatrix() const
{
	return m_CameraMatrix;
}

void Camera2D::Update()
{
	//if (!m_bNeedsUpdate)
	//{
	//	return;
	//}

	//glm::vec3 translate{ -m_Location.x, -m_Location.y, 0 };
	//m_CameraMatrix = glm::translate(m_OrthogonalProjection, translate);

	//glm::vec3 scale{ m_Scale, m_Scale, 0 };
	//m_CameraMatrix = glm::scale( m_OrthogonalProjection, scale );

	//m_bNeedsUpdate = false;
}

glm::mat3 Camera2D::TransformToCameraSpace(glm::mat3 const& transform) const
{
	return m_CameraMatrix * transform;
}