#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class QBert;
class Qube;
class ColoredDisk final : public empire::Component
{
public:
	ColoredDisk(Qube* top);
	ColoredDisk* Clone() override { return new ColoredDisk(*this); }
	void Update() override;

	void ReceivePlayer(QBert* pQbert);
	bool HasQBert() const { return m_bHasQbert; }
private:
	float const OFFSET{-45}; 
	float const DROP_MAX_TIME{ 0.75f };
	float const MOVE_SPEED{ 75.f };
	Qube* const PYRAMID_TOP;
	
	QBert* m_pQbert;
	bool m_bHasQbert;
	bool m_bHasReachedTop;
	float m_DropTimer;

	void Initialize() override {}
};