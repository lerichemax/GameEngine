#include "PCH.h"
#include "MovementComponent.h"
#include "QubeComponent.h"

MovementComponent::MovementComponent()
	:CurrentDirection{ConnectionDirection::null},
	CurrentQube(NULL_ENTITY)
{

}

void MovementComponent::SetTextureIdleNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft)
{
	TextureIdleDownRight = downRight;
	TextureIdleDownLeft = downLeft;
	TextureIdleUpRight = upRight;
	TextureIdleUpLeft = upLeft;
}

void MovementComponent::SetTextureJumpNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft)
{
	TextureJumpDownRight = downRight;
	TextureJumpDownLeft = downLeft;
	TextureJumpUpRight = upRight;
	TextureJumpUpLeft = upLeft;
}
