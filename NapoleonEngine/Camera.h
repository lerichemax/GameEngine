#pragma once

class Camera2D
{
public:
	Camera2D(uint32_t width, uint32_t height);

	void SetPosition(glm::vec2 const& position);
	void SetZoomLevel(float zoom);

	glm::mat3 const& GetCameraMatrix() const;

	void Update();
	glm::mat3 TransformToCameraSpace(glm::mat3 const& transform) const;

private:
	uint32_t m_Width;
	uint32_t m_Height;

	glm::vec2 m_Location;
	float m_Scale;

	bool m_bNeedsUpdate;

	glm::mat3 m_CameraMatrix;
	//glm::mat4 m_OrthogonalProjection;
};
