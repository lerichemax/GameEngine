#pragma once
#include "Component.h"

using namespace empire;

class Qube;
class GameController final : public Component
{
public:
	GameController(unsigned int maxWidth);
	void Init(GameObject* pParent) override;
	Qube* GetTop() const { return m_pQubes.front(); }
	bool AreAllQubesFlipped() const;
private:
	unsigned int const m_MaxWidth;
	
	std::vector<Qube*> m_pQubes;
	
	void Update() override {};

	void CreateConnections();
};