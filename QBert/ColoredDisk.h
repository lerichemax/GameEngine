#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class QBert;
class Qube;
class ColoredDisk final : public Component
{
public:
	ColoredDisk();
	ColoredDisk* Clone() const override { return new ColoredDisk(*this); }

	ColoredDisk(ColoredDisk&& other) = delete;
	ColoredDisk& operator=(ColoredDisk const& other) = delete;
	ColoredDisk& operator=(ColoredDisk&& other) = delete;
	~ColoredDisk() = default;
	
	void Update() override;

	void ReceivePlayer(QBert* pQbert);
	bool HasQBert() const { return m_bHasQbert; }

	void SetPyramidTop(Qube* pTop) { m_pPyramidTop = pTop; }
	static int GetPoints() { return POINTS; }
private:
	float const OFFSET{-45}; 
	float const DROP_MAX_TIME{ 0.75f };
	float const MOVE_SPEED{ 100.f };
	static int const POINTS{ 50 };
	
	Qube* m_pPyramidTop;
	QBert* m_pQbert;
	bool m_bHasQbert;
	bool m_bHasReachedTop;
	float m_DropTimer;

	void Initialize() override {}

	ColoredDisk(ColoredDisk const& other);
};