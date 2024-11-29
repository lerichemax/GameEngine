#pragma once
#include "Component.h"

enum class MovementMode
{
	Normal,
	EscheresqueRight,
	EscheresqueLeft
};

enum class ConnectionDirection;
struct MovementComponent : public ecs::Component
{
	MovementComponent();

	bool bCanMove{ true };

	Entity CurrentQube;

	PROPERTY(std::string, TextureIdleDownRight);
	PROPERTY(std::string, TextureIdleDownLeft);
	PROPERTY(std::string, TextureIdleUpRight);
	PROPERTY(std::string, TextureIdleUpLeft);
						
	PROPERTY(std::string, TextureJumpDownRight);
	PROPERTY(std::string, TextureJumpDownLeft);
	PROPERTY(std::string, TextureJumpUpRight);
	PROPERTY(std::string, TextureJumpUpLeft);

	ConnectionDirection CurrentDirection;
	PROPERTY(MovementMode, Mode);

	void SetTextureIdleNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft);
	void SetTextureJumpNames(std::string const& downRight, std::string const& downLeft, std::string const& upRight, std::string const& upLeft);
};

SERIALIZE_CLASS(MovementComponent, ecs::Component)