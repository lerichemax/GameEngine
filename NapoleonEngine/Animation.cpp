#include "PCH.h"
#include "Animation.h"

Animation::Animation(std::vector<std::string> const& sprites, float timePerSprite)
	:m_AnimationSprites{sprites},
	m_TimePerSprite{timePerSprite}
{

}