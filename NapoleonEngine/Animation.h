#pragma once
#include <vector>
#include <string>

class Animation final
{
	friend class AnimationSystem;
	
public:
	Animation();
	Animation(std::string const& name, std::vector<std::string> const& sprites, float timePerSprite = 0.25f);

	PROPERTY(bool, m_bLoop);

	std::string GetName() const { return m_Name; }

private:
	PROPERTY(std::vector<std::string>, m_AnimationSprites);
	PROPERTY(float, m_TimePerSprite);
	PROPERTY(std::string, m_Name);
};

SERIALIZE_CLASS(Animation)