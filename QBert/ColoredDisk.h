#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class QBert;
class QubeSystem;
class ColoredDisk final : public Component
{
public:
	ColoredDisk();

	ColoredDisk(ColoredDisk&& other) = delete;
	ColoredDisk& operator=(ColoredDisk const& other) = delete;
	ColoredDisk& operator=(ColoredDisk&& other) = delete;
	~ColoredDisk() = default;
	
	void Update();

	void ReceivePlayer(QBert* pQbert);
	bool HasQBert() const { return m_bHasQbert; }

	void SetPyramidTop(QubeSystem* pTop) { m_pPyramidTop = pTop; }
	static int GetPoints() { return POINTS; }
private:
	float const OFFSET{-45}; 
	float const DROP_MAX_TIME{ 0.75f };
	float const MOVE_SPEED{ 100.f };
	static int const POINTS{ 50 };
	
	QubeSystem* m_pPyramidTop;
	QBert* m_pQbert;
	bool m_bHasQbert;
	bool m_bHasReachedTop;
	float m_DropTimer;

	ColoredDisk(ColoredDisk const& other);
};