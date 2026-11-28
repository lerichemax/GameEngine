#pragma once

class ICamera
{
public:
	virtual ~ICamera() = default;
	virtual glm::mat3 TransformIntoCameraSpace(glm::mat3 const& transform) const = 0;
	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;
};

class CameraLocator final
{
public:
	static ICamera* Get() { return m_pCameraService; }
	static void RegisterCamera(ICamera* const pCamera) { m_pCameraService = pCamera; }

private:
	static ICamera* m_pCameraService;
};

class Camera2D final : public ICamera
{
public:
	Camera2D(uint32_t width, uint32_t height);

	void SetPosition(glm::vec2 const& position);
	void SetZoomLevel(float zoom);

	glm::mat3 const& GetCameraMatrix() const;

	void Update();
	glm::mat3 TransformIntoCameraSpace(glm::mat3 const& transform) const override;

	uint32_t GetWidth() const override;
	uint32_t GetHeight() const override;

private:
	uint32_t m_Width;
	uint32_t m_Height;

	glm::vec2 m_Location;
	float m_Scale;

	bool m_bNeedsUpdate;

	glm::mat3 m_CameraMatrix;
	//glm::mat4 m_OrthogonalProjection;
};
