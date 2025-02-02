#include "PCH.h"
#include "Animation.h"

Animation::Animation()
	:m_AnimationSprites{ },
	m_TimePerSprite{ },
	m_Name{ },
	m_bLoop{ true }
{
}

Animation::Animation(std::string const& name, std::vector<std::string> const& sprites, float timePerSprite)
	:m_AnimationSprites{sprites},
	m_TimePerSprite{timePerSprite},
	m_Name{name},
	m_bLoop{true}
{

}