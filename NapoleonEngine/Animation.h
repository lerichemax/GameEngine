#pragma once
#include <vector>
#include <string>

class Animation final
{
public:
	Animation(std::vector<std::string> const& sprites, float timePerSprite = 0.25f);

private:
	std::vector<std::string> m_AnimationSprites;
	float m_TimePerSprite;
};