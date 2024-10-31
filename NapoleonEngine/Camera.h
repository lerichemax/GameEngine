#pragma once

class Camera2D
{
public:
	Camera2D(uint32_t width, uint32_t height);

	void SetPosition(glm::vec2 const& position);
	void SetZoomLevel(float zoom);

	glm::mat4 const& GetCameraMatrix() const;

	void Update();

private:
	uint32_t m_Width;
	uint32_t m_Height;

	glm::vec2 m_Location;
	float m_Scale;

	bool m_bNeedsUpdate;

	glm::mat4 m_CameraMatrix;
	glm::mat4 m_OrthogonalProjection;
};
